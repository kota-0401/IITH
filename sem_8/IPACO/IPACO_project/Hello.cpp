#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <bits/stdc++.h>
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/InstIterator.h"

#include "llvm/Support/Path.h"
#include "llvm/IR/Module.h"
using namespace llvm;
using namespace std;

#define DEBUG_TYPE "Cuda_bug"

STATISTIC(Cuda_bugCounter, "Counts number of functions greeted");

namespace {
  struct Cuda_bug : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    Cuda_bug() : FunctionPass(ID), File("temp.txt", EC, llvm::sys::fs::OF_Text) {}

    /* Constant Prop Data Structures*/
    map<BasicBlock*, map<Value*, Constant*>> bb_var_values;
    map<BasicBlock*, map<Value*, string>> bb_lattice;
    std::error_code EC;
    llvm::raw_fd_ostream File; // Declare the file stream here
    string Input_File;
    map<Value*, Constant*> last_block_var_values; // to store the last block values

    /* Cuda Bug Detection Data Structures */
    map<BasicBlock*, vector<BasicBlock*>> bb_predecessors; 
    map<string, vector<string>> var_dep_on_tid;
    map<BasicBlock*, string> block_name;
    map<BasicBlock*, Instruction*> branchInst_per_block;
    map<Value*, bool> is_val_tid;

    /* Constant Prop Functions */
    void remove_first_line_of_file() {
        ifstream fin("temp.txt");

        // remove last 3 characters from the Input_File
          Input_File.pop_back();
          Input_File.pop_back();
          Input_File.pop_back();

          Input_File += ".txt";
          ofstream fout("./output/" + Input_File);

        string line;
        int count = 0;
        while(getline(fin, line)) {
            if(count == 0) {
                count++;
                continue;
            }
            fout << line << endl;
        }
          fout << "}\n";
          fin.close();
          fout.close();
          // delete temp.txt file
          remove("temp.txt");
    }

    string getRegisterName_Load(Instruction &I){
        // for load instruction
        string load_inst;
        raw_string_ostream rso(load_inst);
        I.print(rso);
        string reg_name;
        for(auto ch : load_inst){
            if(ch == '%'){
                reg_name = "";
            }
            else if(ch == '='){
                break;
            }
            else{
                reg_name += ch;
            }
        }
        reg_name.pop_back();

        return reg_name;
    }

    vector<string> get_var_names(Instruction &I) {
        vector<string> var_names;
        string load_inst;
        raw_string_ostream rso(load_inst);
        I.print(rso);
        string temp;
        for(long unsigned int i = 0; i < load_inst.size(); i++) {
            if(load_inst[i] == '%') {
                temp = "";
                i++;
                while(i != load_inst.size() and (isalpha(load_inst[i]) or isdigit(load_inst[i]))) {
                    temp += load_inst[i];
                    i++;
                }
                var_names.push_back(temp);
            }
            
        }
        return var_names;
    }

    string get_var_names_fromValue(Value *V){
        vector<string> var_names;
        string inst;
        raw_string_ostream rso(inst);
        V->print(rso);
        string temp;
        for(long unsigned int i = 0; i < inst.size(); i++) {
            if(inst[i] == '%') {
                temp = "";
                i++;
                while(i != inst.size() and (isalpha(inst[i]) or isdigit(inst[i]))) {
                    temp += inst[i];
                    i++;
                }
                var_names.push_back(temp);
            }
            
        }
        // if inst contains store, return var_names[1] else return var_names[0]
        if(inst.find("store") != string::npos) {
            return var_names[1];
        }
        else {
            return var_names[0];
        }
    }

    set<Constant*> get_prev_values(vector<BasicBlock*> preds, Value* val) {
        set<Constant*> prev_values;
        for(BasicBlock *pred : preds) {
            if(bb_var_values.find(pred) != bb_var_values.end()) {
                if(bb_var_values[pred][val] != NULL) {
                    prev_values.insert(bb_var_values[pred][val]);
                }
            }
        }
        return prev_values;
    }

    void print_val(map<Value*, Constant*> &temp_var_values, map<Value*, string> &temp_lattice, Value *op) {
        if(temp_lattice[op] == "BOTTOM") {
            errs() << "BOTTOM";
            File << "BOTTOM";
        }
        else if(temp_lattice[op] == "CONSTANT") {
            errs() << temp_var_values[op]->getUniqueInteger();
            File << temp_var_values[op]->getUniqueInteger();
        }
        else {
            errs() << "TOP";
            File << "TOP";
        }
    }

        void AllocaInst_func(Instruction &I, map<Value*, Constant*> &temp_var_values, map<Value*, string> &temp_lattice) {
            errs() << I;
            File << I;
            AllocaInst *allocaInst = dyn_cast<AllocaInst>(&I);
            if (!allocaInst) return;  // Ensure casting was successful

            string varName = allocaInst->getName().str();
            temp_var_values[allocaInst] = NULL;
            temp_lattice[allocaInst] = "TOP";
            // // print varname 
            // errs() << varName << " " << allocaInst << " ";
            errs() << " --> %" << varName << "=TOP\n";
            File << " --> %" << varName << "=TOP\n";          
        }

        void StoreInst_func(Instruction &I, map<Value*, Constant*> &temp_var_values, map<Value*, string> &temp_lattice , vector<BasicBlock*> preds) {
            errs()<< I;
            File << I;
            StoreInst *storeInst = dyn_cast<StoreInst>(&I);
            Value *stored_value = storeInst->getValueOperand();
            Value *pointer = storeInst->getPointerOperand();

            vector<string> var_names = get_var_names(I);

            if(Constant *const_val = dyn_cast<Constant>(stored_value)) { // if the stored value is a constant
                ConstantInt *const_val_int = (ConstantInt*)const_val;
                temp_var_values[pointer] = const_val;
                temp_lattice[pointer] = "CONSTANT";
              //   errs() << " --> %" << pointer->getName().str() << "=" << const_val_int->getUniqueInteger() << "\n";
              //   File << " --> %" << pointer->getName().str() << "=" << const_val_int->getUniqueInteger() << "\n";
                  errs() << " --> %" << var_names[0] << "=" << const_val_int->getUniqueInteger() << "\n";
                  File << " --> %" << var_names[0] << "=" << const_val_int->getUniqueInteger() << "\n";

            }

            else{ // if the stored value is a variable
                vector<string> var_names = get_var_names(I);
                 if(temp_lattice[stored_value] == "BOTTOM") {
                    temp_lattice[pointer] = "BOTTOM";
                    temp_var_values[pointer] = NULL;
                      errs() << " --> %" << var_names[0] << "=BOTTOM, ";
                      errs() << "%" << var_names[1] << "=BOTTOM\n";

                      File << " --> %" << var_names[0] << "=BOTTOM, ";
                      File << "%" << var_names[1] << "=BOTTOM\n";
                 } 
                 else if(temp_lattice[stored_value] == "CONSTANT") {
                    temp_lattice[pointer] = "CONSTANT";
                    temp_var_values[pointer] = temp_var_values[stored_value];
                    errs() << " --> %" << var_names[0] << "=" << (temp_var_values[pointer])->getUniqueInteger() << ", ";
                    errs() << "%" << var_names[1] << "=" << (temp_var_values[pointer])->getUniqueInteger() << "\n";

                    File << " --> %" << var_names[0] << "=" << (temp_var_values[pointer])->getUniqueInteger() << ", ";
                      File << "%" << var_names[1] << "=" << (temp_var_values[pointer])->getUniqueInteger() << "\n";
                 }
                 else if(temp_lattice[stored_value] == "TOP") {
                    temp_lattice[pointer] = "TOP";
                    temp_var_values[pointer] = NULL;
                  //   errs() << " --> %" << stored_value->getName().str() << "=TOP, ";
                  //   errs() << "%" << pointer->getName().str() << "=TOP\n";
                      errs() << " --> %" << var_names[0] << "=TOP, ";
                      errs() << "%" << var_names[1] << "=TOP\n";

                      // File << " --> %" << stored_value->getName().str() << "=TOP, ";
                      // File << "%" << pointer->getName().str() << "=TOP\n";
                      File << " --> %" << var_names[0] << "=TOP, ";
                      File << "%" << var_names[1] << "=TOP\n";
                 }
            }
            errs() << "\n";
              File << "\n";
        }

        void LoadInst_func(Instruction &I, map<Value*, Constant*> &temp_var_values, map<Value*, string> &temp_lattice, vector<BasicBlock*> preds) {
            errs() << I;
            File << I;
            LoadInst *loadInst = dyn_cast<LoadInst>(&I);
            Value *pointer = loadInst->getPointerOperand();

            if(Constant *const_val = dyn_cast<Constant>(pointer)) { // if the stored value is a constant
                ConstantInt *const_val_int = (ConstantInt*)const_val;
                temp_var_values[loadInst] = const_val;

                const APInt temp = const_val_int->getUniqueInteger();
                temp_lattice[loadInst] = "CONSTANT";
                temp_lattice[pointer] = "CONSTANT";

                // first print register info then print the value
                errs() << " --> %" << getRegisterName_Load(I) << "=" << const_val_int->getUniqueInteger() << ", ";
                errs() << "%" << loadInst->getName().str() << "=" << const_val_int->getUniqueInteger() << "\n";

                  File << " --> %" << getRegisterName_Load(I) << "=" << const_val_int->getUniqueInteger() << ", ";
                  File << "%" << loadInst->getName().str() << "=" << const_val_int->getUniqueInteger() << "\n";
            }

            else{ // if the stored value is a variable. Eg:- %1 = load i32, i32* %x
                  vector<string> var_names = get_var_names(I);
                  if(temp_lattice[pointer] == "BOTTOM") {
                    temp_lattice[loadInst] = "BOTTOM";
                    temp_var_values[loadInst] = NULL;
                    errs() << " --> %" << var_names[0] << "=BOTTOM, ";
                    errs() << "%" << var_names[1] << "=BOTTOM\n";

                      File << " --> %" << var_names[0] << "=BOTTOM, ";
                      File << "%" << var_names[1] << "=BOTTOM\n";
                  } 
                  else if(temp_lattice[pointer] == "CONSTANT") {
                    temp_lattice[loadInst] = "CONSTANT";
                    temp_var_values[loadInst] = temp_var_values[pointer];
                    errs() << " --> %" << var_names[0] << "=" << (temp_var_values[loadInst])->getUniqueInteger() << ", ";
                    errs() << "%" << var_names[1] << "=" << (temp_var_values[loadInst])->getUniqueInteger() << "\n";

                      File << " --> %" << var_names[0] << "=" << (temp_var_values[loadInst])->getUniqueInteger() << ", ";
                      File << "%" << var_names[1] << "=" << (temp_var_values[loadInst])->getUniqueInteger() << "\n";
                  }
                  else if(temp_lattice[pointer] == "TOP") {
                    temp_lattice[loadInst] = "TOP";
                    temp_var_values[loadInst] = NULL;
                    errs() << " --> %" << var_names[0] << "=TOP, ";
                    errs() << "%" << var_names[1] << "=TOP\n";

                      File << " --> %" << var_names[0] << "=TOP, ";
                      File << "%" << var_names[1] << "=TOP\n";
                  }
            }
        }

        void BinaryOperator_func(Instruction &I, BinaryOperator *BO, map<Value*, Constant*> &temp_var_values, map<Value*, string> &temp_lattice, vector<BasicBlock*> preds) {
                errs() << I;
                File << I;
                Value *op1 = BO->getOperand(0);
                Value *op2 = BO->getOperand(1);

                ConstantInt *op1_const_int = dyn_cast<ConstantInt>(op1);
                if(op1_const_int) {
                    op1_const_int = dyn_cast<ConstantInt>(op1);
                    temp_var_values[op1] = op1_const_int;
                    temp_lattice[op1] = "CONSTANT";
                }

                ConstantInt *op2_const_int = dyn_cast<ConstantInt>(op2);
                if(op2_const_int) {
                    op2_const_int = dyn_cast<ConstantInt>(op2);
                    temp_var_values[op2] = op2_const_int;
                    temp_lattice[op2] = "CONSTANT";
                }

                vector<string> var_names = get_var_names(I);
                if(temp_lattice[op1] == "BOTTOM" || temp_lattice[op2] == "BOTTOM") {
                    temp_lattice[&I] = "BOTTOM";
                    temp_var_values[&I] = NULL;
                    errs() << " --> %" << var_names[0] << "=BOTTOM, ";
                    File << " --> %" << var_names[0] << "=BOTTOM, ";
                    if (!dyn_cast<Constant>(op1) && !dyn_cast<Constant>(op2)) { // 2 variables
                      //   errs() << "%" << var_names[1] << "=";print_val(temp_var_values, temp_lattice, op1);errs()<<", ";
                      //   errs() << "%" << var_names[2] << "="; print_val(temp_var_values, temp_lattice, op2); errs()<<"\n";

                          File << "%" << var_names[1] << "=";print_val(temp_var_values, temp_lattice, op1);File<<", ";
                          File << "%" << var_names[2] << "="; print_val(temp_var_values, temp_lattice, op2); File<<"\n";
                    } else { // 1 variable
                        errs() << "%" << var_names[1] << "=BOTTOM\n";
                          File << "%" << var_names[1] << "=BOTTOM\n";
                    }
                } 
                else {
                    temp_lattice[&I] = "CONSTANT";
                    APInt result(32, 0, false);
                    if(BO->getOpcode() == Instruction::Add) {
                        result = temp_var_values[op1]->getUniqueInteger() + temp_var_values[op2]->getUniqueInteger();
                    }
                    else if(BO->getOpcode() == Instruction::FAdd) {
                        result = temp_var_values[op1]->getUniqueInteger() + temp_var_values[op2]->getUniqueInteger();
                    }
                    else if(BO->getOpcode() == Instruction::Sub) {
                        result = temp_var_values[op1]->getUniqueInteger() - temp_var_values[op2]->getUniqueInteger();
                    }
                    else if(BO->getOpcode() == Instruction::Mul) {
                        result = temp_var_values[op1]->getUniqueInteger() * temp_var_values[op2]->getUniqueInteger();
                    }
                    else if(BO->getOpcode() == Instruction::SDiv) {
                        result = temp_var_values[op1]->getUniqueInteger().sdiv(temp_var_values[op2]->getUniqueInteger());
                    }
                    else if(BO->getOpcode() == Instruction::SRem) {
                        result = temp_var_values[op1]->getUniqueInteger().srem(temp_var_values[op2]->getUniqueInteger());
                    }

                    temp_var_values[&I] = ConstantInt::get(I.getContext(), result);
                    errs() << " --> %" << var_names[0] << "=" << result << ", ";
                    File << " --> %" << var_names[0] << "=" << result << ", ";
                    if (!dyn_cast<Constant>(op1) && !dyn_cast<Constant>(op2)) {
                        errs() << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << ", ";
                        errs() << "%" << var_names[2] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";

                          File << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << ", ";
                          File << "%" << var_names[2] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";
                    }
                    else if(!dyn_cast<Constant>(op1)) {
                        errs() << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << "\n";
                          File << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << "\n";
                    }
                    else if(!dyn_cast<Constant>(op2)) {
                        errs() << "%" << var_names[1] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";
                          File << "%" << var_names[1] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";
                    }
                    else {
                        errs() << "\n";
                          File << "\n";
                    }
                }
                errs() << "\n";
                  File << "\n";
                
        }

        void ComparisonOperator_func(Instruction &I, map<Value*, Constant*> &temp_var_values, map<Value*, string> &temp_lattice, vector<BasicBlock*> preds) {
            errs() << I;
              File << I;
            ICmpInst *icmpInst = dyn_cast<ICmpInst>(&I);
            Value *op1 = icmpInst->getOperand(0);
            Value *op2 = icmpInst->getOperand(1);
            ConstantInt *op1_const_int = dyn_cast<ConstantInt>(op1);
            if(op1_const_int) {
                op1_const_int = dyn_cast<ConstantInt>(op1);
                temp_var_values[op1] = op1_const_int;
                temp_lattice[op1] = "CONSTANT";
            }
            
            ConstantInt *op2_const_int = dyn_cast<ConstantInt>(op2);
            if(op2_const_int) {
                op2_const_int = dyn_cast<ConstantInt>(op2);
                temp_var_values[op2] = op2_const_int;
                temp_lattice[op2] = "CONSTANT";
            }

            vector<string> var_names = get_var_names(I);
            if(temp_lattice[op1] == "BOTTOM" || temp_lattice[op2] == "BOTTOM") {
                temp_lattice[&I] = "BOTTOM";
                temp_var_values[&I] = NULL;
                errs() << " --> %" << var_names[0] << "=BOTTOM, ";
                  File << " --> %" << var_names[0] << "=BOTTOM, ";
                if (!dyn_cast<Constant>(op1) && !dyn_cast<Constant>(op2)) {
                    errs() << "%" << var_names[1] << "=";print_val(temp_var_values, temp_lattice, op1);errs()<<", ";
                    errs() << "%" << var_names[2] << "="; print_val(temp_var_values, temp_lattice, op2); errs()<<"\n";

                      File << "%" << var_names[1] << "=";print_val(temp_var_values, temp_lattice, op1);File<<", ";
                      File << "%" << var_names[2] << "="; print_val(temp_var_values, temp_lattice, op2); File<<"\n";
                } else {
                    errs() << "%" << var_names[1] << "=BOTTOM\n";
                      File << "%" << var_names[1] << "=BOTTOM\n";
                }
            } 
            else {
                temp_lattice[&I] = "CONSTANT";
                APInt result(2, 0, false);
                if(icmpInst->getPredicate() == CmpInst::ICMP_EQ) {
                    result = APInt(2, temp_var_values[op1]->getUniqueInteger() == temp_var_values[op2]->getUniqueInteger(), false);
                }
                else if(icmpInst->getPredicate() == CmpInst::ICMP_NE) {
                    result = APInt(2, temp_var_values[op1]->getUniqueInteger() != temp_var_values[op2]->getUniqueInteger(), false);
                }
                else if(icmpInst->getPredicate() == CmpInst::ICMP_SGT) {
                    result = APInt(2, temp_var_values[op1]->getUniqueInteger().sgt(temp_var_values[op2]->getUniqueInteger()), false);
                }
                else if(icmpInst->getPredicate() == CmpInst::ICMP_SGE) {
                    result = APInt(2, temp_var_values[op1]->getUniqueInteger().sge(temp_var_values[op2]->getUniqueInteger()), false);
                }
                else if(icmpInst->getPredicate() == CmpInst::ICMP_SLT) {
                    result = APInt(2, temp_var_values[op1]->getUniqueInteger().slt(temp_var_values[op2]->getUniqueInteger()), false);
                }
                else if(icmpInst->getPredicate() == CmpInst::ICMP_SLE) {
                    result = APInt(2, temp_var_values[op1]->getUniqueInteger().sle(temp_var_values[op2]->getUniqueInteger()), false);
                }

                temp_var_values[&I] = ConstantInt::get(I.getContext(), result);
                errs() << " --> %" << var_names[0] << "=" << result << ", ";
                  File << " --> %" << var_names[0] << "=" << result << ", ";
                if (!dyn_cast<Constant>(op1) && !dyn_cast<Constant>(op2)) {
                    errs() << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << ", ";
                    errs() << "%" << var_names[2] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";

                      File << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << ", ";
                      File << "%" << var_names[2] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";
                }
                else if(!dyn_cast<Constant>(op1)) {
                    errs() << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << "\n";
                      File << "%" << var_names[1] << "=" << temp_var_values[op1]->getUniqueInteger() << "\n";
                }
                else if(!dyn_cast<Constant>(op2)) {
                    errs() << "%" << var_names[1] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";
                      File << "%" << var_names[1] << "=" << temp_var_values[op2]->getUniqueInteger() << "\n";
                }
                else {
                    errs() << "\n";
                      File << "\n";
                }
            }
        }

        void RetOperator_func(Instruction &I, map<Value*, Constant*> &var_values, map<Value*, string> &lattice) {
          errs() << I;
          File << I;
      
          ReturnInst *retInst = dyn_cast<ReturnInst>(&I);
          if (!retInst) return;
      
          Value *retVal = retInst->getReturnValue();
      
          // Handle 'ret void'
          if (!retVal) {
              errs() << "\n";
              File << "\n";
              return;
          }
      
          vector<string> var_names = get_var_names(I);
          Constant *const_val = dyn_cast<Constant>(retVal);
      
          if (const_val != nullptr) {
              errs() << "\n";
              File << "\n";
          } else {
              if (lattice[retVal] == "BOTTOM") {
                  errs() << " --> %" << var_names[0] << "=BOTTOM\n";
                  File << " --> %" << var_names[0] << "=BOTTOM\n";
              } else {
                  errs() << " --> %" << var_names[0] << "=" << var_values[retVal]->getUniqueInteger() << "\n";
                  File << " --> %" << var_names[0] << "=" << var_values[retVal]->getUniqueInteger() << "\n";
              }
          }
      }    
        
        void CallInst_func(Instruction &I, map<Value*, Constant*> &var_values, map<Value*, string> &lattice, vector<BasicBlock*> preds) {
            errs() << I;
            File << I;
            CallInst *callInst = dyn_cast<CallInst>(&I);
            Function *calledFunction = callInst->getCalledFunction();
            vector<string> var_names = get_var_names(I);

            if(calledFunction->getName() == "printf") {
                vector<Value*> operands;
                for(unsigned int i = 1; i < callInst->getNumOperands() - 1; i++) {
                    operands.push_back(callInst->getOperand(i));
                }

                for(unsigned int i = 0; i < operands.size(); i++) {
                    // if the operand is a constant just print the value
                    if(Constant *const_val = dyn_cast<Constant>(operands[i])) {
                        errs() << " --> %" << var_names[i+1] << "=" << const_val->getUniqueInteger();
                        File << " --> %" << var_names[i+1] << "=" << const_val->getUniqueInteger();
                    }
                    else {
                        if(lattice[operands[i]] == "BOTTOM") {
                            if(i == 0) errs() << " --> %" << var_names[i+1] << "=BOTTOM";
                            else errs() << ", %" << var_names[i+1] << "=BOTTOM";

                              if(i == 0) File << " --> %" << var_names[i+1] << "=BOTTOM";
                              else File << ", %" << var_names[i+1] << "=BOTTOM";
                        }
                        else {
                            if(i == 0) errs() << " --> %" << var_names[i+1] << "=" << var_values[operands[i]]->getUniqueInteger();
                            else errs() << ", %" << var_names[i+1] << "=" << var_values[operands[i]]->getUniqueInteger();

                              if(i == 0) File << " --> %" << var_names[i+1] << "=" << var_values[operands[i]]->getUniqueInteger();
                              else File << ", %" << var_names[i+1] << "=" << var_values[operands[i]]->getUniqueInteger();
                        }
                    }
                }
                errs() << "\n";
                  File << "\n";
            }

            else if(calledFunction->getName() == "__isoc99_scanf") {
                vector<Value*> operands;
                for(unsigned int i = 1; i < callInst->getNumOperands() - 1; i++) {
                    operands.push_back(callInst->getOperand(i));
                }
                for(unsigned int i = 0; i < operands.size(); i++) {
                    lattice[operands[i]] = "BOTTOM";
                    var_values[operands[i]] = NULL;
                    if(i == 0) errs() << " --> %" << var_names[i+1] << "=BOTTOM ";
                    else errs() << ", %" << var_names[i+1] << "=BOTTOM";

                      if(i == 0) File << " --> %" << var_names[i+1] << "=BOTTOM ";
                      else File << ", %" << var_names[i+1] << "=BOTTOM";
                }
                errs() << "\n";
                  File << "\n";
            }

            else if(calledFunction->getName().startswith("llvm.nvvm.read.ptx.sreg.ctaid")) {
                  var_values[&I] = ConstantInt::get(I.getContext(), APInt(32, 0, false));
                  lattice[&I] = "CONSTANT";
                  errs() << " --> %" << var_names[0] << "=0\n";
                    File << " --> %" << var_names[0] << "=0\n";
            }
            else if (calledFunction && calledFunction->getName().startswith("llvm.nvvm.read.ptx.sreg.ntid")) {
            //   var_values[&I] = ConstantInt::get(I.getContext(), APInt(32, 1, false));
            //   lattice[&I] = "CONSTANT";
                 var_values[&I] = ConstantInt::get(I.getContext(), APInt(32, 0, false));
                 lattice[&I] = "CONSTANT";
            
              errs() << " --> %" << var_names[0] << "=" << "0\n";
                File << " --> %" << var_names[0] << "=" << "0\n";
          }
            else if(calledFunction->getName().startswith("llvm.nvvm.read.ptx.sreg.tid")) {
              var_values[&I] = ConstantInt::get(I.getContext(), APInt(32, 0, false));
              lattice[&I] = "CONSTANT";
            
              errs() << " --> %" << var_names[0] << "=" << "0\n";
                File << " --> %" << var_names[0] << "=" << "0\n";
            }
          
            errs() << "\n\n";         
        }

        void BranchInst_func(Instruction &I, map<Value*, Constant*> &var_values, map<Value*, string> &lattice, vector<BasicBlock*> preds){
            errs() << I;
              File << I;
            BranchInst *branchInst = dyn_cast<BranchInst>(&I);
            if(branchInst->isConditional()) {
                Value *condition = branchInst->getCondition();
                vector<string> var_names = get_var_names(I);
                if(lattice[condition] == "BOTTOM") {
                    lattice[condition] = "BOTTOM";
                    var_values[condition] = NULL;
                    errs() << " --> %" << var_names[0] << "=BOTTOM\n";
                    File << " --> %" << var_names[0] << "=BOTTOM\n";
                }
                else if(lattice[condition] == "CONSTANT") {
                    lattice[condition] = "CONSTANT";
                    var_values[condition] = var_values[condition];
                    errs() << " --> %" << var_names[0] << "=" << var_values[condition]->getUniqueInteger() << "\n";
                      File << " --> %" << var_names[0] << "=" << var_values[condition]->getUniqueInteger() << "\n";
                }
            }
            else {

            errs() << "\n";
            File << "\n";

            }
        }

        void Pointer_instruction(Instruction &I, map<Value*, Constant*> &var_values, map<Value*, string> &lattice, vector<BasicBlock*> preds) {
            errs() << I;
            File << I;
            GetElementPtrInst *getelementptrInst = dyn_cast<GetElementPtrInst>(&I);
            Value *pointer = getelementptrInst->getPointerOperand();

            lattice[pointer] = "BOTTOM";
            var_values[pointer] = NULL;
            lattice[&I] = "BOTTOM";
            var_values[&I] = NULL;
            errs() << " --> %" << pointer->getName().str() << "=BOTTOM, ";
            errs() << "%" << getRegisterName_Load(I) << "=BOTTOM\n";

              File << " --> %" << pointer->getName().str() << "=BOTTOM, ";
              File << "%" << getRegisterName_Load(I) << "=BOTTOM\n";
        }

        void Switch_Instruction(Instruction &I, map<Value*, Constant*> &var_values, map<Value*, string> &lattice, vector<BasicBlock*> preds){
            errs() << I;
              File << I;
            SwitchInst *switchInst = dyn_cast<SwitchInst>(&I);
            Value *condition = switchInst->getCondition();
            vector<string> var_names = get_var_names(I);
            if(lattice[condition] == "BOTTOM") {
                lattice[condition] = "BOTTOM";
                var_values[condition] = NULL;
                errs() << " --> %" << var_names[0] << "=BOTTOM\n";
                  File << " --> %" << var_names[0] << "=BOTTOM\n";
            }
            else if(lattice[condition] == "CONSTANT") {
                lattice[condition] = "CONSTANT";
                var_values[condition] = var_values[condition];
                errs() << " --> %" << var_names[0] << "=" << var_values[condition]->getUniqueInteger() << "\n";
                  File << " --> %" << var_names[0] << "=" << var_values[condition]->getUniqueInteger() << "\n";
            }

        }

    void run_interior_func(Function &F, int &counter, bool &all_block_maps_same) {
        errs() << "Iteration : " << counter << "\n";
        // File << "Iteration : " << counter << "\n";
        all_block_maps_same = false;
        // print function name
        errs() << "Function Name: " << F.getName() << "\n";

            // print function name
            errs() << "Function Name: " << F.getName() << "\n";
            File << "define dso_local i32 @main() #0 {"<< "\n";

            for(BasicBlock &BB : F){
                map<Value*, Constant*> var_values;
                map<Value*, string> lattice;

                // print block name
                errs()<< BB.getName() << "\n";
                File << BB.getName() << "\n";

                // assign function arguments Values to BOTTOM
                for (auto &arg : F.args()) {
                    lattice[&arg] = "BOTTOM";
                    var_values[&arg] = NULL;
                }

                vector<BasicBlock*> preds;
                for(BasicBlock *pred : predecessors(&BB)) {
                    preds.push_back(pred);
                }
            
                    for(BasicBlock *pred : preds) {
                        for(auto it = bb_var_values[pred].begin(); it != bb_var_values[pred].end(); it++) {
                            if(var_values.find(it->first) == var_values.end()) {
                                var_values[it->first] = it->second;
                                lattice[it->first] = bb_lattice[pred][it->first];
                            }
                            else if((var_values[it->first] != it->second) or
                            (bb_lattice[pred][it->first] != lattice[it->first])
                            ) {
                                var_values[it->first] = NULL;
                                lattice[it->first] = "BOTTOM";
                            }
                        }
                    }
                    

                for(Instruction &I : BB) {
                        if (I.getOpcode() == Instruction::Alloca) {
                            AllocaInst_func(I, var_values, lattice);
                        }

                        else if (I.getOpcode() == Instruction::Store) {
                            StoreInst_func(I, var_values, lattice, preds);
                        }

                        else if(I.getOpcode() == Instruction::Load){
                            LoadInst_func(I, var_values, lattice, preds);
                        }

                        else if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
                            BinaryOperator_func(I, BO, var_values, lattice, preds);
                        }

                        else if(I.getOpcode() == Instruction::ICmp) {
                            ComparisonOperator_func(I, var_values, lattice, preds);
                        }

                        // else if(I.getOpcode() == Instruction::Ret) {
                        //     RetOperator_func(I, var_values, lattice);
                        // }

                        else if(I.getOpcode() == Instruction::Call) {
                            CallInst_func(I, var_values, lattice, preds);
                        }

                        else if(I.getOpcode() == Instruction::Br) {
                            BranchInst_func(I, var_values, lattice, preds);
                        }

                        else if(I.getOpcode() == Instruction::GetElementPtr) {
                            Pointer_instruction(I, var_values, lattice, preds);
                        }

                        else if(I.getOpcode() == Instruction::Switch) {
                            Switch_Instruction(I, var_values, lattice, preds);
                        }
                }
                //   errs() << " ********************************************************\n";
                //  // print the var_values map and lattice map
                //  for(auto it = var_values.begin(); it != var_values.end(); it++) {
                //      errs() << it->first << " " << it->second << "\n";
                //  }
                //  errs() << " ********************************************************\n";

                bool same = false;
                // checking if both the lattice and value maps of previous iteration and current iteration are equal or not
                if(bb_var_values[&BB] != var_values and bb_lattice[&BB] != lattice) {
                    same = true;
                }
                all_block_maps_same |= same;
        
                bb_var_values[&BB] = var_values;
                bb_lattice[&BB] = lattice;
            }
            counter++;
            errs() << "}\n";
    }    

    void applyConstantPropagation(Function &F) {
        Module *M = F.getParent();
        string input_file_name = llvm::sys::path::filename(M->getModuleIdentifier()).str();
        Input_File = input_file_name;
        errs() << "*********************\n";
        errs() << "input file name: " << input_file_name << "\n";
        errs() << "*********************\n";
        int counter = 0;
        bool all_block_maps_same = false;
        do{
            run_interior_func(F, counter, all_block_maps_same);
            } while((all_block_maps_same && counter < 100));
        
        counter--;
        llvm::raw_fd_ostream File("temp.txt", EC, llvm::sys::fs::OF_None); // clearing the previous content of the file
        run_interior_func(F, counter, all_block_maps_same); // printing the final iteration of the unchanged values
        remove_first_line_of_file(); // removing the first line of the file (some random thing is getting printed)

        // print final map bb_var_values, bb_lattice
        errs() << "Final Map of Values and Lattice\n";
        errs() << "size of bb_var_values: " << bb_var_values.size() << "\n";
        // for(auto it = bb_var_values.begin(); it != bb_var_values.end(); it++) {
        //     if(it->first == NULL) continue;
        //     for(auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
        //         if(it1->first == NULL or it1->second == NULL) continue;
        //         Value *val = it1->first;
        //         string first_arg_val;
        //         raw_string_ostream rso(first_arg_val);
        //         val->print(rso);
        //         errs() << "Value: " << rso.str() << " --> " << (it1->second)->getUniqueInteger() << "\n";

        //         // errs() << " --> " << it1->first->getName().str() << " : " << (it1->second)->getUniqueInteger() << "\n";
        //     }
        // }

        // Printing the last block values

        if (!bb_var_values.empty()) {
            auto last_block_it = std::prev(bb_var_values.end()); // get iterator to last element
            BasicBlock* last_bb = last_block_it->first;
        
            if (last_bb != nullptr) {
                for (auto it1 = last_block_it->second.begin(); it1 != last_block_it->second.end(); ++it1) {
                    if (it1->first == nullptr) continue;

                    Value* val = it1->first;
                    string first_arg_val;
                    raw_string_ostream rso(first_arg_val);
                    val->print(rso);
                    // if(it1->second == NULL) continue;
                    if(it1->second == NULL) errs() << "Value: " << rso.str() << " --> BOTTOM\n";
                    else 
                    errs() << "Value: " << rso.str() << " --> " << it1->second->getUniqueInteger() << "\n";
                }
            }
        }
        
    }

    /* Cuda Bug Detection Functions */
    vector<string> get_pointerArguments(Function &F) {
        vector<string> args;
        unsigned idx = 0;
        for (auto &arg : F.args()) {
            if (arg.getType()->isPointerTy()) {
                string argName = arg.getName().str();
                if (argName.empty()) {
                    argName = to_string(idx);
                }
                args.push_back(argName);
            }
            idx++;
        }
        return args;
    }

    void set_Block_and_Predecessor_names(Function &F) {
            int bbIndex = 0;

            // First pass: assign names to unnamed blocks
            for (BasicBlock &BB : F) {
              if (!BB.hasName()) {
                // BB.setName("bb" + std::to_string(bbIndex++));
                block_name[&BB] = "bb" + std::to_string(bbIndex++);
              }
            }
    }

        void getRegisterName(Instruction &I) {
          // for load instruction
          string inst;
          raw_string_ostream rso(inst);
          I.print(rso);
          errs() << "Instruction: " << inst << "\n";
      }

      vector<string> get_operands(Instruction &I) {
          vector<string> operands;
          string inst;
          raw_string_ostream rso(inst);
          I.print(rso);
          string temp;
          for(long unsigned int i = 0; i < inst.size(); i++) {
              if(inst[i] == '%') {
                  temp = "";
                  i++;
                  while(i != inst.size() and (isalpha(inst[i]) or isdigit(inst[i]))) {
                      temp += inst[i];
                      i++;
                  }
                  operands.push_back(temp);
              }
          }
          return operands;
      }

      void buildTidDependencyMap(Instruction &I) {
        // load instruction
         if (auto *loadInst = dyn_cast<LoadInst>(&I)) {
            vector<string> operands = get_operands(I);
            var_dep_on_tid[operands[0]].push_back(operands[1]);
         }
        // store instruction
         else if (auto *storeInst = dyn_cast<StoreInst>(&I)) {
            vector<string> operands = get_operands(I);
            // errs() << "Instruction Thread dep: " << I << "\n";
            // errs()  << "Operands: ";
            // for (const auto &op : operands) {
            //     errs() << op << " ";
            // }
            // errs() << "\n";
            if(operands.size() > 1) {
                var_dep_on_tid[operands[1]].push_back(operands[0]);
            }
            else var_dep_on_tid[operands[0]].push_back(operands[0]);
         }
        // binary operator
         else if(auto *binaryOp = dyn_cast<BinaryOperator>(&I)) {
            vector<string> operands = get_operands(I);
            // errs() << "Instruction Thread dep: " << I << "\n";
            // errs()  << "Operands: ";
            // for (const auto &op : operands) {
            //     errs() << op << " ";
            // }
            // errs() << "\n";
            for(int i = 1; i < operands.size(); i++) var_dep_on_tid[operands[0]].push_back(operands[i]);
         }

         // sext instruction
         else if (auto *sextInst = dyn_cast<SExtInst>(&I)) {
            vector<string> operands = get_operands(I);
            var_dep_on_tid[operands[0]].push_back(operands[1]);
         }

         // sitof instruction
         else if (auto *sitofInst = dyn_cast<SIToFPInst>(&I)) {
            vector<string> operands = get_operands(I);
            var_dep_on_tid[operands[0]].push_back(operands[1]);
         }

         // getelementptr instruction
         else if (auto *getElementPtrInst = dyn_cast<GetElementPtrInst>(&I)) {
            vector<string> operands = get_operands(I);
            var_dep_on_tid[operands[0]].push_back(operands[1]);
            var_dep_on_tid[operands[0]].push_back(operands[2]);
         }

         // compare instruction
         else if (auto *compareInst = dyn_cast<ICmpInst>(&I)) {
            vector<string> operands = get_operands(I);
            for(int i = 1; i < operands.size(); i++) var_dep_on_tid[operands[0]].push_back(operands[i]);
         }

         else if (auto *callInst = dyn_cast<CallInst>(&I)) {
            Function *calledFunction = callInst->getCalledFunction();
            // if (calledFunction && calledFunction->getName() == "llvm.nvvm.barrier0") {
            //     // Handle barrier function call
            // }
            if(calledFunction && calledFunction->getName().startswith("llvm.nvvm.read.ptx.sreg.tid")) {
                vector<string> operands = get_operands(I);
                var_dep_on_tid[operands[0]].push_back(calledFunction->getName().str());
            }
            if(calledFunction && calledFunction->getName().startswith("llvm.nvvm.read.ptx.sreg.ctaid")) {
                vector<string> operands = get_operands(I);
                var_dep_on_tid[operands[0]].push_back(calledFunction->getName().str());
            }
            if(calledFunction && calledFunction->getName().startswith("llvm.nvvm.read.ptx.sreg.ntid")) {
                vector<string> operands = get_operands(I);
                var_dep_on_tid[operands[0]].push_back(calledFunction->getName().str());
            }
         }

         // handle fcmp instruction
         else if (auto *fcmpInst = dyn_cast<FCmpInst>(&I)) {
            vector<string> operands = get_operands(I);
            for(int i = 1; i < operands.size(); i++) var_dep_on_tid[operands[0]].push_back(operands[i]);
         }
      }

      bool check_InstDependencyOnBlockID(Instruction &I) {
        vector<string> operands = get_operands(I);
        if (operands.size() < 2) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
        string check_variable;
        // if the instruction is not store, then check_variable is the first operand
        // else for the store instruction, check_variable is the second operand
        if (I.getOpcode() == Instruction::Store) {
            check_variable = operands[1];
        } else {
            check_variable = operands[0];
        }
        dfs_stack.push(check_variable);
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Base case: direct block ID access
            if (curr == "llvm.nvvm.read.ptx.sreg.ctaid.x" ||
                curr == "llvm.nvvm.read.ptx.sreg.ctaid.y" ||
                curr == "llvm.nvvm.read.ptx.sreg.ctaid.z") {
                return true;
            }
    
            // Recurse on dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
      }
      
      bool check_InstDependencyOnBlockIDim(Instruction &I) {
        vector<string> operands = get_operands(I);
        if (operands.size() < 2) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
        string check_variable;
        // if the instruction is not store, then check_variable is the first operand
        // else for the store instruction, check_variable is the second operand
        if (I.getOpcode() == Instruction::Store) {
            check_variable = operands[1];
        } else {
            check_variable = operands[0];
        }
        dfs_stack.push(check_variable);
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Base case: direct block ID access
            if (curr == "llvm.nvvm.read.ptx.sreg.ntid.x" ||
                curr == "llvm.nvvm.read.ptx.sreg.ntid.y" ||
                curr == "llvm.nvvm.read.ptx.sreg.ntid.z") {
                return true;
            }
    
            // Recurse on dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
      }

      bool check_VarDependencyOnBlockIDim(string s) {
        if (s.empty()) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
        dfs_stack.push(s);
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Base case: direct block ID access
            if (curr == "llvm.nvvm.read.ptx.sreg.ntid.x" ||
                curr == "llvm.nvvm.read.ptx.sreg.ntid.y" ||
                curr == "llvm.nvvm.read.ptx.sreg.ntid.z") {
                return true;
            }
    
            // Recurse on dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
      }

      bool check_VarDependencyOnBlockID(string s) {
        if (s.empty()) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
        dfs_stack.push(s);
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Base case: direct block ID access
            if (curr == "llvm.nvvm.read.ptx.sreg.ctaid.x" ||
                curr == "llvm.nvvm.read.ptx.sreg.ctaid.y" ||
                curr == "llvm.nvvm.read.ptx.sreg.ctaid.z") {
                return true;
            }
    
            // Recurse on dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
      }

      bool check_InstDependencyOnTid(Instruction &I) {
        vector<string> operands = get_operands(I);
        if (operands.size() < 2) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
        string check_variable;
        // if the instruction is not store, then check_variable is the first operand
        // else for the store instruction, check_variable is the second operand
        if (I.getOpcode() == Instruction::Store) {
            check_variable = operands[1];
        } else {
            check_variable = operands[0];
        }


        dfs_stack.push(check_variable);
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Base case: direct thread ID access
            if (curr == "llvm.nvvm.read.ptx.sreg.tid.x" ||
                curr == "llvm.nvvm.read.ptx.sreg.tid.y" ||
                curr == "llvm.nvvm.read.ptx.sreg.tid.z") {
                return true;
            }
    
            // Recurse on dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
      }

      bool check_VarDependencyOnTid(string s) {
        if (s.empty()) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
        dfs_stack.push(s);
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Base case: direct thread ID access
            if (curr == "llvm.nvvm.read.ptx.sreg.tid.x" ||
                curr == "llvm.nvvm.read.ptx.sreg.tid.y" ||
                curr == "llvm.nvvm.read.ptx.sreg.tid.z") {
                return true;
            }
    
            // Recurse on dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
      }
    
      bool checkDependencyOnSharedMem(Instruction &I, vector<string> &args_of_func) {
        vector<string> operands = get_operands(I);
        if (operands.empty()) return false;
    
        set<string> visited;
        stack<string> dfs_stack;
    
        for (const string &op : operands) {
            if (!op.empty())
                dfs_stack.push(op);
        }
    
        while (!dfs_stack.empty()) {
            string curr = dfs_stack.top();
            dfs_stack.pop();
    
            if (curr.empty() || visited.count(curr)) continue;
            visited.insert(curr);
    
            // Check if current operand is a function argument (e.g., "0", "1", ...)
            if (std::find(args_of_func.begin(), args_of_func.end(), curr) != args_of_func.end()) {
                return true;
            }
    
            // Recurse through variable dependencies
            if (var_dep_on_tid.find(curr) != var_dep_on_tid.end()) {
                for (const string &dep : var_dep_on_tid[curr]) {
                    if (!dep.empty())
                        dfs_stack.push(dep);
                }
            }
        }
    
        return false;
    }

    string getSymbol_of_BInaryOPerator(Instruction &I) {
        string symbol;
        if (I.getOpcode() == Instruction::Add) {
            symbol = "+";
        } else if (I.getOpcode() == Instruction::Sub) {
            symbol = "-";
        } else if (I.getOpcode() == Instruction::Mul) {
            symbol = "*";
        } else if (I.getOpcode() == Instruction::SDiv or I.getOpcode() == Instruction::UDiv or I.getOpcode() == Instruction::FDiv) {
            symbol = "/";
        }
        // } else if (I.getOpcode() == Instruction::URem) {
        //     symbol = "%";
        // } else if (I.getOpcode() == Instruction::And) {
        //     symbol = "&";
        // } else if (I.getOpcode() == Instruction::Or) {
        //     symbol = "|";
        // } else if (I.getOpcode() == Instruction::Xor) {
        //     symbol = "^";
        // }
        return symbol;
    }

    string getSymbol_of_ComparisonOperator(Instruction &I) {
        string symbol;
        // if it is slt or sgt or gt or lt or eq or ne etc
        if (I.getOpcode() == Instruction::ICmp) {
            ICmpInst *icmpInst = dyn_cast<ICmpInst>(&I);
            if (icmpInst->getPredicate() == CmpInst::ICMP_EQ) {
                symbol = "==";
            } else if (icmpInst->getPredicate() == CmpInst::ICMP_NE) {
                symbol = "!=";
            } else if (icmpInst->getPredicate() == CmpInst::ICMP_SGT) {
                symbol = ">";
            } else if (icmpInst->getPredicate() == CmpInst::ICMP_SLT) {
                symbol = "<";
            } else if (icmpInst->getPredicate() == CmpInst::ICMP_SGE) {
                symbol = ">=";
            } else if (icmpInst->getPredicate() == CmpInst::ICMP_SLE) {
                symbol = "<=";
            }
        }
        return symbol;
    }

    string ConvertValToString(Value *val) {
        string str;
        raw_string_ostream rso(str);
        val->print(rso);
        errs() << "Value: " << rso.str() << "\n";

        string ans = rso.str();
        // traverse from the end of string until you find space
        for (int i = ans.size() - 1; i >= 0; i--) {
            if (ans[i] == ' ') {
                ans = ans.substr(i+1, ans.size() - i-1);
                break;
            }
        }
        return ans;
    }

    // string resolveExpr(Value *V, map<Value *, string> &dp, map<Value *, Value *> &memoryMap) {
    //             if (dp.count(V)) return dp[V];

    //             if (auto *C = dyn_cast<ConstantInt>(V)) {
    //                 return to_string(C->getSExtValue());
    //             }

    //             if (auto *CI = dyn_cast<CallInst>(V)) {
    //                 StringRef FName = CI->getCalledFunction()->getName();
    //                 if (FName.contains("ctaid.x")) return "blockIdx.x";
    //                 if (FName.contains("ntid.x"))  return "blockDim.x";
    //                 if (FName.contains("tid.x"))   return "threadIdx.x";
    //                 return "<call>";
    //             }

    //             if (auto *BO = dyn_cast<BinaryOperator>(V)) {
    //                 string L = resolveExpr(BO->getOperand(0), dp, memoryMap);
    //                 string R = resolveExpr(BO->getOperand(1), dp, memoryMap);
    //                 string op;
    //                 switch (BO->getOpcode()) {
    //                 case Instruction::Add:  op = "+"; break;
    //                 case Instruction::Sub:  op = "-"; break;
    //                 case Instruction::Mul:  op = "*"; break;
    //                 case Instruction::SDiv: op = "/"; break;
    //                 case Instruction::UDiv: op = "/"; break;
    //                 case Instruction::URem: op = "%"; break;
    //                 case Instruction::And:  op = "&"; break;
    //                 case Instruction::Or:   op = "|"; break;
    //                 default: op = "?"; break;
    //                 }
    //                 string expr = "(" + L + " " + op + " " + R + ")";
    //                 dp[V] = expr;
    //                 return expr;
    //             }

    //             if (auto *LI = dyn_cast<LoadInst>(V)) {
    //                 Value *ptr = LI->getPointerOperand();
    //                 if (memoryMap.count(ptr)) { // 
    //                 return resolveExpr(memoryMap[ptr], dp, memoryMap);
    //                 } else {
    //                 return "<load-unknown>";
    //                 }
    //             }

    //             return "<unknown>";
    // }

// Recursive expression resolver using global data
        // string resolveOperand(Value *V, BasicBlock *BB) {
        //         // Handle constant integers
        //         if (auto *C = dyn_cast<ConstantInt>(V)) {
        //             return to_string(C->getSExtValue()) + "(const)";
        //         }

        //         // Get variable name for dependency checks
        //         string instName;
        //         raw_string_ostream rso(instName);
        //         V->print(rso);
        //         instName = rso.str();
        //         string varName = get_var_names_fromValue(V);
        //         errs() << "Instruction: " << instName << "\n";

        //         // if varname has alloca, then find the bb_var_values of that variable
        //         if (instName.find("alloca") != string::npos) {
        //             if(bb_var_values[BB].count(V)) {
        //                 // if bb_lattice is BOTTOM, then print bottom
        //                 if (bb_lattice[BB][V] == "BOTTOM") {
        //                     errs() << " Value is :" << "BOTTOM" << "\n";
        //                 }
        //                 else if (bb_lattice[BB][V] == "CONSTANT") {
        //                     Constant *C = bb_var_values[BB][V];
        //                     if (ConstantInt *CI = dyn_cast<ConstantInt>(C)) {
        //                         errs() << " Value is :" << bb_var_values[BB][V]->getUniqueInteger() << "\n";
        //                     }
        //                 }
        //             }

        //             bool flag = check_VarDependencyOnTid(varName);
        //             errs() << "Dependency on thread id: " << flag << "\n";
        //         }

        //         // Check dependencies
        //         bool hasDependency = check_VarDependencyOnTid(varName) || 
        //                             check_VarDependencyOnBlockID(varName) ||
        //                             check_VarDependencyOnBlockIDim(varName);

                
        //     if(!hasDependency) {
        //         errs() << "Resolving1: " << varName << "\n";
        //         // Check for constant values first
        //         if (bb_lattice[BB].count(V) && bb_lattice[BB][V] == "CONSTANT") {
        //             Constant *C = bb_var_values[BB][V];
        //             if (ConstantInt *CI = dyn_cast<ConstantInt>(C)) {
        //                 return to_string(CI->getSExtValue());
        //             }
        //         }
        //         // check for bottom values
        //         if (bb_lattice[BB].count(V) && bb_lattice[BB][V] == "BOTTOM") {
        //             return "BOTTOM";
        //         }
        //         return "TOP";
        //     }
            
        //     errs() << "Resolving2: " << varName << "\n";

        //         // Handle thread/block intrinsics directly
        //         if (CallInst *CI = dyn_cast<CallInst>(V)) {
        //             Function *Callee = CI->getCalledFunction();
        //             if (Callee && Callee->getName().startswith("llvm.nvvm.read.ptx.sreg.")) {
        //                 return Callee->getName().substr(23).str();
        //             }
        //         }

        //         // Check dependencies
        //         hasDependency = check_VarDependencyOnTid(varName) || 
        //                             check_VarDependencyOnBlockID(varName) ||
        //                             check_VarDependencyOnBlockIDim(varName);

        //                                  // if alloca instruction, then print
        //             if (instName.find("alloca") != string::npos) {
        //                 errs() << "\nalloca(" << ")\n";
        //             }

        //         // Only backtrack if dependent on thread/block variables
        //         if (hasDependency) {
        //             if (BinaryOperator *BO = dyn_cast<BinaryOperator>(V)) {
        //                 string LHS = resolveOperand(BO->getOperand(0), BB);
        //                 string RHS = resolveOperand(BO->getOperand(1), BB);
                        
        //                 const char *op = "";
        //                 switch (BO->getOpcode()) {
        //                     case Instruction::Add: op = "+"; break;
        //                     case Instruction::Sub: op = "-"; break;
        //                     case Instruction::Mul: op = "*"; break;
        //                     case Instruction::SDiv: 
        //                     case Instruction::UDiv: op = "/"; break;
        //                     default: op = "?";
        //                 }

        //                 return "(" + LHS + " " + op + " " + RHS + ")";
        //             }
                    
        //             // // Handle PHI nodes and other instructions
        //             if (Instruction *I = dyn_cast<Instruction>(V)) {
        //                 vector<string> operands;
        //                 for (Use &U : I->operands()) {
        //                     operands.push_back(resolveOperand(U.get(), BB));
        //                 }
        //             }
        //             // // handle alloca instruction
        //             // if (AllocaInst *AI = dyn_cast<AllocaInst>(V)) {
        //             //     return resolveOperand(AI, BB);
        //             // }
        //         }

        //         // Return raw value if no dependencies or unresolved
        //         return varName;
        // }
        set<Value *> getPossibleStoredValues(Value *ptr, BasicBlock *BB,
            map<BasicBlock *, map<Value *, set<Value *>>> &memoryMapPerBlock) {
            set<Value *> results;
            for (BasicBlock *Pred : predecessors(BB)) {
                if (memoryMapPerBlock[Pred].count(ptr)) {
                    const auto &vals = memoryMapPerBlock[Pred][ptr];
                    results.insert(vals.begin(), vals.end());
                }
            }
            return results;
        }
        
        string resolveExpr(Value *V, map<Value *, string> &dp,
                           map<BasicBlock *, map<Value *, set<Value *>>> &memoryMapPerBlock,
                           BasicBlock *BB) {
            if (dp.count(V)) return dp[V];
        
            if (auto *C = dyn_cast<ConstantInt>(V)) {
                return to_string(C->getSExtValue()) + "(const)";
            }
        
            if (auto *CI = dyn_cast<CallInst>(V)) {
                StringRef FName = CI->getCalledFunction()->getName();
                if (FName.contains("ctaid.x"))  return "blockIdx.x";
                if (FName.contains("ntid.x"))   return "blockDim.x";
                if (FName.contains("tid.x"))    return "threadIdx.x";
                return "<call>";
            }
        
            if (auto *BO = dyn_cast<BinaryOperator>(V)) {
                if (!check_InstDependencyOnTid(*BO)) {
                    if (check_InstDependencyOnBlockID(*BO) && check_InstDependencyOnBlockIDim(*BO)) {
                        return "blockIdx.x * blockDim.x";
                    }
                    if (bb_var_values[BB].count(V)) {
                        string Instr;
                        raw_string_ostream rso(Instr);
                        V->print(rso);
                        errs() << "Touched Instruction: " << rso.str() << "\n";
        
                        if (ConstantInt *C = dyn_cast_or_null<ConstantInt>(bb_var_values[BB][V])) {
                            return to_string(C->getSExtValue());
                        } else if (bb_lattice[BB].count(V)) {
                            return bb_lattice[BB][V];
                        } else {
                            return "<no-lattice>";
                        }
                    } else {
                        return "<no-prop>";
                    }
                }
        
                string L = resolveExpr(BO->getOperand(0), dp, memoryMapPerBlock, BB);
                string R = resolveExpr(BO->getOperand(1), dp, memoryMapPerBlock, BB);
                string op = getSymbol_of_BInaryOPerator(*BO);
                string expr = "(" + L + " " + op + " " + R + ")";
                dp[V] = expr;
                return expr;
            }
        
            if (auto *LI = dyn_cast<LoadInst>(V)) {
                Value *ptr = LI->getPointerOperand();
                set<Value *> candidates = getPossibleStoredValues(ptr, LI->getParent(), memoryMapPerBlock);
                
                if (candidates.empty()) return "<load-unknown>";
        
                set<string> resolved;
                for (Value *val : candidates) {
                    resolved.insert(resolveExpr(val, dp, memoryMapPerBlock, BB));
                }
        
                if (resolved.size() == 1) return resolved.begin()->c_str();
                // return "phi(" + join(resolved, ", ") + ")";
                return "bottom";
            }
        
            if (!check_InstDependencyOnTid(*cast<Instruction>(V))) {
                if (bb_var_values[BB].count(V)) {
                    if (ConstantInt *C = dyn_cast_or_null<ConstantInt>(bb_var_values[BB][V])) {
                        return to_string(C->getSExtValue());
                    } else if (bb_lattice[BB].count(V)) {
                        return bb_lattice[BB][V];
                    } else {
                        return "<no-lattice>";
                    }
                } else {
                    return "<no-prop>";
                }
            }
        
            return "<unknown>";
        }
        

            //    // Main analysis function
            //    void analyzeComparisons(Function &F) {
            //     for (BasicBlock &BB : F) {
            //     for (Instruction &I : BB) {
            //         if (CmpInst *CI = dyn_cast<CmpInst>(&I)) {
            //         Value *LHS = CI->getOperand(0);
            //         Value *RHS = CI->getOperand(1);
                    
            //         string lhsExpr = resolveOperand(LHS, &BB);
            //         string rhsExpr = resolveOperand(RHS, &BB);
                    
            //         // Format comparison with actual predicate
            //         errs() << lhsExpr << " " 
            //                 << CmpInst::getPredicateName(CI->getPredicate()).str() << " "
            //                 << rhsExpr << "\n";
            //         }
            //       }
            //     }
            // }
    
    void calculateThreadRange(BasicBlock &BB) {
        // get predecessor block of BB
        vector<BasicBlock*> preds;
        for (BasicBlock *pred : predecessors(&BB)) {
            preds.push_back(pred);
        }
        // if preds.size() >= 2, then print check the calculateThreadRnage Logic
        if(preds.size() >= 2) {
            errs() << "Check Thread Range Calculation Logic\n";
            return;
        }
        else if(preds.size() == 1) {
            stack<pair<string, string>> range_stack;
            for(auto &I : *preds[0]) {
                // ignore load and store instructions
                if (I.getOpcode() == Instruction::Load || I.getOpcode() == Instruction::Store) {
                    continue;
                }
                // if it is a binary operator, then one operand should be dependent on thread-id and other should have a value, else ignore
                if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
                    Value *op1 = BO->getOperand(0);
                    Value *op2 = BO->getOperand(1);
                    vector<string> operands = get_operands(I);
                    string symbol_op = getSymbol_of_BInaryOPerator(I);

                    // first check if any of the operand are constant
                    if (auto *const_val = dyn_cast<Constant>(op1)) {
                        range_stack.push(make_pair(getSymbol_of_BInaryOPerator(I), ConvertValToString(op1)));
                        continue;
                    }
                    else if (auto *const_val = dyn_cast<Constant>(op2)) {
                        range_stack.push(make_pair(getSymbol_of_BInaryOPerator(I), ConvertValToString(op2)));
                        continue;
                    }

                    bool dependency_on_op1 = check_VarDependencyOnTid(operands[1]);
                    bool dependency_on_op2 = check_VarDependencyOnTid(operands[2]);
                    if(dependency_on_op1 and !dependency_on_op2) {
                        Value *val_op2 = bb_var_values[preds[0]][op2];
                        string value2;
                        if(val_op2 == NULL) {
                            value2 = "BOTTOM";
                        }
                        else {
                            APInt val = bb_var_values[preds[0]][op2]->getUniqueInteger();
                            raw_string_ostream rso(value2);
                            val.print(rso, true);
                            value2 = rso.str();
                        }
                        range_stack.push(make_pair(getSymbol_of_BInaryOPerator(I), value2));
                    }
                    else if(!dependency_on_op1 and dependency_on_op2) {
                        Value *val_op1 = bb_var_values[preds[0]][op1];
                        string value1;
                        if(val_op1 == NULL) {
                            value1 = "BOTTOM";
                        }
                        else {
                            APInt val = bb_var_values[preds[0]][op1]->getUniqueInteger();
                            raw_string_ostream rso(value1);
                            val.print(rso, true);
                            value1 = rso.str();
                        }
                        range_stack.push(make_pair(getSymbol_of_BInaryOPerator(I), value1));
                    }
                }
                // else if it is a comparison operator
                else if(auto *cmp = dyn_cast<ICmpInst>(&I)) {
                    Value *op1 = cmp->getOperand(0);
                    Value *op2 = cmp->getOperand(1);
                    vector<string> operands = get_operands(I);
                    string symbol_op = getSymbol_of_ComparisonOperator(I);
                    // first check if any of the operand are constant
                    if (auto *const_val = dyn_cast<Constant>(op1)) {
                        range_stack.push(make_pair(getSymbol_of_ComparisonOperator(I), ConvertValToString(op1)));
                        continue;
                    }
                    else if (auto *const_val = dyn_cast<Constant>(op2)) {
                        range_stack.push(make_pair(getSymbol_of_ComparisonOperator(I), ConvertValToString(op2)));
                        continue;
                    }
                    bool dependency_on_op1 = check_VarDependencyOnTid(operands[1]);
                    bool dependency_on_op2 = check_VarDependencyOnTid(operands[2]);
                    if(dependency_on_op1 and !dependency_on_op2) {
                        Value *val_op2 = bb_var_values[preds[0]][op2];
                        string value2;
                        if(val_op2 == NULL) {
                            value2 = "BOTTOM";
                        }
                        else {
                            APInt val = bb_var_values[preds[0]][op2]->getUniqueInteger();
                            raw_string_ostream rso(value2);
                            val.print(rso, true);
                            value2 = rso.str();
                        }
                        range_stack.push(make_pair(getSymbol_of_ComparisonOperator(I), value2));
                    }
                    else if(!dependency_on_op1 and dependency_on_op2) {
                        Value *val_op1 = bb_var_values[preds[0]][op1];
                        string value1;
                        if(val_op1 == NULL) {
                            value1 = "BOTTOM";
                        }
                        else {
                            APInt val = bb_var_values[preds[0]][op1]->getUniqueInteger();
                            raw_string_ostream rso(value1);
                            val.print(rso, true);
                            value1 = rso.str();
                        }
                        range_stack.push(make_pair(getSymbol_of_ComparisonOperator(I), value1));
                    }
                }
            }

            // print the range_stack
            errs() << "Range Stack: ";
            while(!range_stack.empty()) {
                pair<string, string> p = range_stack.top();
                range_stack.pop();
                errs() << "{" << p.first << "," << p.second << "} ";
            }

            // pair<string, string> comp_pair = range_stack.top();
            // string comp_symbol = comp_pair.first;
            // string main_value = comp_pair.second;
            // range_stack.pop();
            // while(range_stack.size() > 1) {
            //     pair<string, string> p_temp = range_stack.top();
            //     range_stack.pop();
            //     string op_symbol = p_temp.first;
            //     string value_temp = p_temp.second;
            //     if(op_symbol == "+") {
            //         main_value = to_string(stoi(main_value) - stoi(value_temp));
            //     }
            //     else if(op_symbol == "-") {
            //         main_value = to_string(stoi(main_value) + stoi(value_temp));
            //     }
            //     else if(op_symbol == "*") {
            //         main_value = to_string(stoi(main_value) / stoi(value_temp));
            //     }
            //     else if(op_symbol == "/") {
            //         main_value = to_string(stoi(main_value) * stoi(value_temp));
            //     }
            // }
            // errs() << "Range of thread: " << comp_symbol << " " << main_value << "\n";
        }
    }

      void check_divergence_in_Block(Function &F, vector<string> &args_of_func) {
        for (auto &BB : F) {
          // Skip blocks with a return instruction
          if (isa<ReturnInst>(BB.getTerminator())) {
              continue;
          }

          errs() << "Basic Block: " << block_name[&BB] << "\n";
          bool hasBarrierDivergence = false;
          
          // check if the current block has any call instruction which are syncthread calls
          for (Instruction &I : BB) {
              if (auto *callInst = dyn_cast<CallInst>(&I)) {
                  Function *calledFunction = callInst->getCalledFunction();
                  // errs() << "function called: " << calledFunction->getName() << "\n";
                  if (calledFunction && calledFunction->getName() == "llvm.nvvm.barrier0") {
                      hasBarrierDivergence = true;
                      break; // Exit the loop if a syncthread call is found
                  }
              }
          }
      
          if(!hasBarrierDivergence) {
            errs() << "No Barrier Divergence\n";
          }

          else { // check if the condition statement depends on thread-id.
                vector<BasicBlock*> preds;
                for (BasicBlock *pred : predecessors(&BB)) {
                    preds.push_back(pred);
                }
                // check if any of the predecessors have a thread id dependency
                  bool hasThreadIdDependency_on_func_args = false;
                  bool hasThreadIdDependency = false;

                  for (BasicBlock *pred : preds) {
                      // Check if pred exists in the map
                      if (branchInst_per_block.find(pred) != branchInst_per_block.end()) {
                          Instruction *branchInst = branchInst_per_block[pred];
                          // Check if it’s actually a BranchInst (safe cast)
                          if (auto *br = dyn_cast<BranchInst>(branchInst)) {
                              if (check_InstDependencyOnTid(*br)) {
                                hasThreadIdDependency = true;
                                 if (checkDependencyOnSharedMem(*br, args_of_func)) {
                                    hasThreadIdDependency_on_func_args = true;
                                    break; // Exit the loop if a thread ID dependency is found
                                 }
                              }
                          }
                      }
                  }

                if(hasThreadIdDependency_on_func_args and hasThreadIdDependency) {
                    errs() << "Barrier Divergence\n";
                }
                else if(!hasThreadIdDependency) {
                    errs() << "No Barrier Divergence\n";
                }
                else {
                    calculateThreadRange(BB);
                    errs() << "\n\n";
                }
            }
      }      
    }

    bool runOnFunction(Function &F) override {
        applyConstantPropagation(F);
        
/*Good code Below: */
        // set_Block_and_Predecessor_names(F); // we have block names as register-names so I am setting the block names to bb0, bb1, bb2, etc.

        // /*
        // step-1: if any conditional block doesnot have any syncthread function calls, then we just print it as barrier divergence free 
        // step-2: if any condition statement depends on thread id.
        // */

        // for(auto &BB : F) {
        //     for (Instruction &I : BB) {
        //       buildTidDependencyMap(I);
        //       // if instruction is a branch instruction, then store that instruction in the branchInst_per_block map
        //       if (auto *branchInst = dyn_cast<BranchInst>(&I)) {
        //           branchInst_per_block[&BB] = &I;
        //       }
        //     }
        // }

        // for (auto &BB : F) {
        //     for (Instruction &I : BB) {
        //         if(check_InstDependencyOnTid(I)) {
        //             errs() << I << ": " << "Thread ID dependency found\n";
        //             // if instruction is store, then store the pointer operand in is_val_tid
        //             if (auto *storeInst = dyn_cast<StoreInst>(&I)) {
        //                 Value *ptr = storeInst->getPointerOperand();
        //                 // if (isa<AllocaInst>(ptr)) {
        //                     is_val_tid[ptr] = true;
        //                 // }
        //             }
        //             else {
        //                 // directly assign Instruction to is_val_tid
        //                 is_val_tid[&I] = true;
        //             }
        //         }
        //     }
        // }


        // Resolve each comparison instruction (ICmpInst)
            // Usage in main analysis
            // map<Value *, string> dp;
            // map<BasicBlock *, map<Value *, set<Value *>>> memoryMapPerBlock;

            // for (Instruction &I : instructions(F)) {
            //     if (auto *store = dyn_cast<StoreInst>(&I)) {
            //         memoryMapPerBlock[store->getParent()][store->getPointerOperand()]
            //             .insert(store->getValueOperand());
            //     }
            // }

            // BasicBlock *last_block = std::prev(bb_var_values.end())->first;
            // for (Instruction &I : instructions(F)) {
            //     if (auto *icmp = dyn_cast<ICmpInst>(&I)) {
            //         BasicBlock *BB = icmp->getParent();
            //         string LHS = resolveExpr(icmp->getOperand(0), dp, memoryMapPerBlock, last_block);
            //         string RHS = resolveExpr(icmp->getOperand(1), dp, memoryMapPerBlock, last_block);
            //         string pred = ICmpInst::getPredicateName(icmp->getPredicate()).str();
            //         errs() << "Comparison: " << LHS << " " << pred << " " << RHS << "\n";
            //     }
            // }



        // vector<string> args_of_func = get_pointerArguments(F);
        // check_divergence_in_Block(F, args_of_func);


      return false;
    }
  };
}

char Cuda_bug::ID = 0;
static RegisterPass<Cuda_bug> X("Cuda_bug_pass", "Cuda_bug World Pass");
