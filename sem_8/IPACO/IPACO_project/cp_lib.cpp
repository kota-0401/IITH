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

#include "llvm/Support/Path.h"
#include "llvm/IR/Module.h"
using namespace std;
using namespace llvm;

#define DEBUG_TYPE "hello"

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
struct constant_p : public FunctionPass {
  static char ID;
  constant_p() : FunctionPass(ID), File("temp.txt", EC, llvm::sys::fs::OF_Text) {}

    map<BasicBlock*, map<Value*, Constant*>> bb_var_values;
    map<BasicBlock*, map<Value*, string>> bb_lattice;
    std::error_code EC;
    llvm::raw_fd_ostream File; // Declare the file stream here
    string Input_File;

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

              if(Constant *const_val = dyn_cast<Constant>(stored_value)) { // if the stored value is a constant
                  ConstantInt *const_val_int = (ConstantInt*)const_val;
                  temp_var_values[pointer] = const_val;
                  temp_lattice[pointer] = "CONSTANT";
                  errs() << " --> %" << pointer->getName().str() << "=" << const_val_int->getUniqueInteger() << "\n";
                  File << " --> %" << pointer->getName().str() << "=" << const_val_int->getUniqueInteger() << "\n";
              }

              else{ // if the stored value is a variable
                  vector<string> var_names = get_var_names(I);
                   if(temp_lattice[stored_value] == "BOTTOM") {
                      temp_lattice[pointer] = "BOTTOM";
                      temp_var_values[pointer] = NULL;
                      errs() << " --> %" << stored_value->getName().str() << "=BOTTOM, ";
                      errs() << "%" << pointer->getName().str() << "=BOTTOM\n";

                      File << " --> %" << stored_value->getName().str() << "=BOTTOM, ";
                      File << "%" << pointer->getName().str() << "=BOTTOM\n";
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
                      errs() << " --> %" << stored_value->getName().str() << "=TOP, ";
                      errs() << "%" << pointer->getName().str() << "=TOP\n";

                        File << " --> %" << stored_value->getName().str() << "=TOP, ";
                        File << "%" << pointer->getName().str() << "=TOP\n";
                   }
              }
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

          void RetOperator_func (Instruction &I, map<Value*, Constant*> &var_values, map<Value*, string> &lattice) {
              errs() << I;
              File << I;
              ReturnInst *retInst = dyn_cast<ReturnInst>(&I);
              Value *retVal = retInst->getReturnValue();
              vector<string> var_names = get_var_names(I);
              Constant *const_val = dyn_cast<Constant>(retVal);
              if(const_val != NULL) { // if the stored value is a constant
                  errs() << "\n";
                    File << "\n";
              }
              else { // if the stored value is a variable
                  if(lattice[retVal] == "BOTTOM") {  
                      errs() << " --> %" << var_names[0] << "=BOTTOM\n";
                        File << " --> %" << var_names[0] << "=BOTTOM\n";
                  }
                  else {
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

              else { // just print the function arguments if called.
                  errs() << " \n\n rand function \n\n" ;
                    File << " \n\n rand function \n\n" ;
              }                
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
          // Need To Clarify
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
    
                            else if(I.getOpcode() == Instruction::Ret) {
                                RetOperator_func(I, var_values, lattice);
                            }
    
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

      bool runOnFunction(Function &F) override {
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
          return false;
    }
}; // end of struct constant_p
}  // end of anonymous namespace

char constant_p::ID = 0;
static RegisterPass<constant_p> X("libCP_given", "Constant Propagation Pass for Assignment",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
