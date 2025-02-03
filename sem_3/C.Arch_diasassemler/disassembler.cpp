#include <iostream>
#include <bits/stdc++.h> 
using namespace std;

vector<int> Rformat = {0,1,1,0,0,1,1};
vector<int> Iformat = {0,0,1,0,0,1,1};
vector<int> I_format = {0,0,0,0,0,1,1};
vector<int> I__format = {1,1,0,0,1,1,1};
vector<int> Sformat = {0,1,0,0,0,1,1};
vector<int> Bformat = {1,1,0,0,0,1,1};
vector<int> Jformat = {1,1,0,1,1,1,1};
vector<int> Uformat = {0,1,1,0,1,1,1};

struct command {
    string a;
};

vector<command> result;

int binary_to_decimal (vector<int> binary) {
    int decimal = 0,power = 1;
      for (int i=binary.size()-1;i>=0;i--) {
        decimal += binary.at(i)* power;
        power *= 2;
      }
    return decimal;
}

vector<int> hex_to_binary_vector (string hex) {
    vector<int> binary;
    for (int i = 0; i < hex.size(); i++)
    {   
        if (hex[i]=='0')
        {
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(0);
        }
        else if (hex[i]=='1')
        {
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(1);
        }
        else if (hex[i]=='2')
        {
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(0);
        }
        else if (hex[i]=='3')
        {
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(1);
        }
        else if (hex[i]=='4')
        {
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(0);
        }
        else if (hex[i]=='5')
        {
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(1);
        }
        else if (hex[i]=='6')
        {
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(0);
        }
        else if (hex[i]=='7')
        {
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(1);
        }
        else if (hex[i]=='8')
        {
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(0);
        }
        else if (hex[i]=='9')
        {
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(0);
            binary.push_back(1);
        }
        else if (hex[i]=='a')
        {
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(0);
        }
        else if (hex[i]=='b')
        {
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(1);
            binary.push_back(1);
        }
        else if (hex[i]=='c')
        {
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(0);
        }
        else if (hex[i]=='d')
        {
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(0);
            binary.push_back(1);
        }
        else if (hex[i]=='e')
        {
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(0);
        }
        else if (hex[i]=='f') 
        {
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(1);
            binary.push_back(1);
        }
    }
    
    return binary;
};

void Rtype (vector<int> binary) {
   vector<int> rd = {binary.end()-12, binary.end()-7};
   vector<int> funct3 = {binary.end()-15, binary.end()-12};
   vector<int> rs1 = {binary.end()-20, binary.end()-15};
   vector<int> rs2 = {binary.end()-25, binary.end()-20};
   vector<int> funct7 = {binary.end()-32, binary.end()-25};
   int a,b,c;
    a =   binary_to_decimal(rd);
    b =   binary_to_decimal(rs1);
    c =   binary_to_decimal(rs2);
   if ((funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) && funct7.at(1) == 0) {
       //add
       command add;
       add.a = "add x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(add); 
   }
   else if ((funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) && funct7.at(1) == 1) {
       //sub
       command sub;
       sub.a = "sub x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(sub);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 0) && funct7.at(1) == 0) {
       //xor
       command xor_;
       xor_.a = "xor x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(xor_);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 0) && funct7.at(1) == 0) {
       //or
       command or_;
       or_.a = "or x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(or_);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 1) && funct7.at(1) == 0) {
       //and
       command and_;
       and_.a = "and x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(and_);
   }
   else if ((funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 1) && funct7.at(1) == 0) {
       //sll
       command sll;
       sll.a = "sll x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(sll);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 1) && funct7.at(1) == 0) {
       //srl
       command srl;
       srl.a = "srl x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(srl);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 1) && funct7.at(1) == 1) {
       //sra
       command sra;
       sra.a = "sra x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(sra);
   }
   else if ((funct3.at(0) == 0 && funct3.at(1) == 1 && funct3.at(2) == 0) && funct7.at(1) == 0) {
       //slt
       command slt;
       slt.a = "slt x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(slt);
   }
   else {
       //sltu
       command sltu;
       sltu.a = "sltu x" + to_string(a) + ", x" + to_string(b) + ", x"  + to_string(c);
       result.push_back(sltu);
   } 
}

void Itype (vector<int> binary) {
   vector<int> rd = {binary.end()-12, binary.end()-7};
   vector<int> funct3 = {binary.end()-15, binary.end()-12};
   vector<int> rs1 = {binary.end()-20, binary.end()-15};
   vector<int> imm = {binary.end()-32, binary.end()-20};
   int a,b,c;
    a =   binary_to_decimal(rd);
    b =   binary_to_decimal(rs1);
    c =   binary_to_decimal(imm);
   if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //addi
       command addi;
       addi.a = "addi x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(addi);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //xori
       command xori;
       xori.a = "xori x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(xori);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 0) {
       //ori
       command ori;
       ori.a = "ori x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(ori);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 1) {
       //andi
       command andi;
       andi.a = "andi x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(andi);
   }
   else if ((funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 1) && imm.at(5) == 0) {
       //slli
       command slli;
       slli.a = "slli x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(slli);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 1) && imm.at(5) == 0) {
       //srli
       command srli;
       srli.a = "srli x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(srli);
   }
   else if ((funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 1) && imm.at(5) == 1) {
       //srai
       command srai;
       srai.a = "srai x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(srai);
   }
   else if ((funct3.at(0) == 0 && funct3.at(1) == 1 && funct3.at(2) == 0) && imm.at(1) == 0) {
       //slti
       command slti;
       slti.a = "slti x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(slti);
   }
   else {
       //sltiu
       command sltiu;
       sltiu.a = "sltiu x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(sltiu);
   } 
}

void I_type (vector<int> binary) {
   vector<int> rd = {binary.end()-12, binary.end()-7};
   vector<int> funct3 = {binary.end()-15, binary.end()-12};
   vector<int> rs1 = {binary.end()-20, binary.end()-15};
   vector<int> imm = {binary.end()-32, binary.end()-20};
   int a,b,c;
    a =   binary_to_decimal(rd);
    b =   binary_to_decimal(rs1);
    c =   binary_to_decimal(imm);
   if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //lb
       command lb;
       lb.a = "lb x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(lb);
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 1) {
       //lh
       command lh;
       lh.a = "lh x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(lh);
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 1 && funct3.at(2) == 0) {
       //lw
       command lw;
       lw.a = "lw x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(lw);
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 1 && funct3.at(2) == 1) {
       //ld
       command ld;
       ld.a = "ld x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(ld);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //lbu
       command lbu;
       lbu.a = "lbu x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(lbu);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 1) {
       //lhu
       command lhu;
       lhu.a = "lhu x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(lhu);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 0) {
       //lwu
       command lwu;
       lwu.a = "lwu x" + to_string(a) + ", x" + to_string(b) + ", "  + to_string(c);
       result.push_back(lwu);
   } 
}

void I__type (vector<int> binary) {
   vector<int> rd = {binary.end()-12, binary.end()-7};
   vector<int> funct3 = {binary.end()-15, binary.end()-12};
   vector<int> rs1 = {binary.end()-20, binary.end()-15};
   vector<int> imm = {binary.end()-32, binary.end()-20};
   int a,b,c;
    a =   binary_to_decimal(rd);
    b =   binary_to_decimal(rs1);
    c =   binary_to_decimal(imm);
   if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //jalr
       command jalr;
       jalr.a = "jalr x" + to_string(a) + ", " + to_string(c) + "("  + to_string(b) + ")";
       result.push_back(jalr); 
   } 
}

void Stype (vector<int> binary) {
   vector<int> imm1 = {binary.end()-12, binary.end()-7};
   vector<int> funct3 = {binary.end()-15, binary.end()-12};
   vector<int> rs1 = {binary.end()-20, binary.end()-15};
   vector<int> rs2 = {binary.end()-25, binary.end()-20};
   vector<int> imm2 = {binary.end()-32, binary.end()-25};
   vector<int> imm;
   imm.reserve(imm1.size() + imm2.size());
   imm.insert(imm.end(), imm2.begin(), imm2.end());
   imm.insert(imm.end(), imm1.begin(), imm1.end());
   int a,b,c;
    a =   binary_to_decimal(rs2);
    b =   binary_to_decimal(rs1);
    c =   binary_to_decimal(imm);
   if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //sb
       command sb;
       sb.a = "sb x" + to_string(a) + ", " + to_string(c) + "("  + to_string(b) + ")";
       result.push_back(sb);
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 1) {
       //sh
       command sh;
       sh.a = "sh x" + to_string(a) + ", " + to_string(c) + "("  + to_string(b) + ")";
       result.push_back(sh);
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 1 && funct3.at(2) == 0) {
       //sw
       command sw;
       sw.a = "sw x" + to_string(a) + ", " + to_string(c) + "("  + to_string(b) + ")";
       result.push_back(sw);
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 1 && funct3.at(2) == 1) {
       //sd
       command sd;
       sd.a = "sd x" + to_string(a) + ", " + to_string(c) + "("  + to_string(b) + ")";
       result.push_back(sd);
   } 
}

void Btype (vector<int> binary) {
   vector<int> imm1 = {binary.end()-12, binary.end()-8};
   vector<int> funct3 = {binary.end()-15, binary.end()-12};
   vector<int> rs1 = {binary.end()-20, binary.end()-15};
   vector<int> rs2 = {binary.end()-25, binary.end()-20};
   vector<int> imm2 = {binary.end()-31, binary.end()-25};
   vector<int> imm3 = {binary.at(0)};
   vector<int> imm4 = {binary.at(24)};   
   vector<int> imm5;
   imm5.reserve(imm1.size() + imm2.size());
   imm5.insert(imm5.end(), imm2.begin(), imm2.end());
   imm5.insert(imm5.end(), imm1.begin(), imm1.end()); 
   vector<int> imm6;
   imm6.reserve(imm4.size() + imm3.size());
   imm6.insert(imm6.end(), imm3.begin(), imm3.end());
   imm6.insert(imm6.end(), imm4.begin(), imm4.end());
   vector<int> imm;
   imm.reserve(imm5.size() + imm6.size());
   imm.insert(imm.end(), imm6.begin(), imm6.end());
   imm.insert(imm.end(), imm5.begin(), imm5.end());

   int a,b,c;
    a =   binary_to_decimal(rs2);
    b =   binary_to_decimal(rs1);
    c =   binary_to_decimal(imm);
    
   if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //beq
       command beq;
       beq.a = "beq x" + to_string(b) + ", x " + to_string(a) + ", "  + to_string(b);
       result.push_back(beq); 
   }
   else if (funct3.at(0) == 0 && funct3.at(1) == 0 && funct3.at(2) == 1) {
       //bne
       command bne;
       bne.a = "bne x" + to_string(b) + ", x " + to_string(a) + ", "  + to_string(b);
       result.push_back(bne);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 0) {
       //blt
       command blt;
       blt.a = "blt x" + to_string(b) + ", x " + to_string(a) + ", "  + to_string(b);
       result.push_back(blt);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 0 && funct3.at(2) == 1) {
       //bge
       command bge;
       bge.a = "bge x" + to_string(b) + ", x " + to_string(a) + ", "  + to_string(b);
       result.push_back(bge);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 0) {
       //bltu
       command bltu;
       bltu.a = "bltu x" + to_string(b) + ", x " + to_string(a) + ", "  + to_string(b);
       result.push_back(bltu);
   }
   else if (funct3.at(0) == 1 && funct3.at(1) == 1 && funct3.at(2) == 1) {
       //bgeu
       command bgeu;
       bgeu.a = "bgeu x" + to_string(b) + ", x " + to_string(a) + ", "  + to_string(b);
       result.push_back(bgeu);
   } 
}

void Jtype (vector<int> binary) {
   vector<int> rd = {binary.end()-12, binary.end()-7};
   vector<int> imm4 = {binary.end()-31, binary.end()-21};
   vector<int> imm1 = {binary.end()-20, binary.end()-12};
   vector<int> imm2 = {binary.at(0)};
   vector<int> imm3 = {binary.at(11)};
   vector<int> imm6;
   imm6.reserve(imm1.size() + imm2.size());
   imm6.insert(imm6.end(), imm2.begin(), imm2.end());
   imm6.insert(imm6.end(), imm1.begin(), imm1.end());
   vector<int> imm5;
   imm5.reserve(imm4.size() + imm3.size());
   imm5.insert(imm5.end(), imm3.begin(), imm3.end());
   imm5.insert(imm5.end(), imm4.begin(), imm4.end());
   vector<int> imm;
   imm.reserve(imm5.size() + imm6.size());
   imm.insert(imm.end(), imm6.begin(), imm6.end());
   imm.insert(imm.end(), imm5.begin(), imm5.end());

   int a,b;
   a =   binary_to_decimal(rd);
   b =   binary_to_decimal(imm);
       //jal
       command jal;
       jal.a = "jal x" + to_string(a) + ",  " + to_string(b);
       result.push_back(jal);
    
}

void Utype (vector<int> binary) {
   vector<int> rd = {binary.end()-12, binary.end()-7};
   vector<int> imm = {binary.end()-32, binary.end()-12};
   int a,b;
   a =   binary_to_decimal(rd);
   b =   binary_to_decimal(imm);
       //lui
       command lui;
       lui.a = "lui x" + to_string(a) + ",  " + to_string(b);
       result.push_back(lui);
}

int main(){
    fstream input;
    string filename;
    cin>> filename;
    fstream output;
    string outputname;
    cin>> outputname;
    input.open(filename,ios::in);
    string hex;
    vector<int> binary;
    vector<int> opcode;
    while (getline(input,hex))
    {
        binary = hex_to_binary_vector(hex);
        opcode = {binary.end()-7, binary.end()};
       
        if (opcode==Rformat)
        {
            Rtype(binary);
        }
        else if (opcode==Iformat)
        {
            Itype(binary);
        }
        else if (opcode==I_format)
        {
            I_type(binary);
        }
        else if (opcode==I__format)
        {
            I__type(binary);
        }
        else if (opcode==Sformat)
        {
            Stype(binary);
        }
        else if (opcode==Bformat)
        {
            Btype(binary);
        }
        else if (opcode==Jformat)
        {
            Jtype(binary);
        }
        else if (opcode==Uformat)
        {
            Utype(binary);
        }
        else 
        {
            cout<< "error";
        }
        
    }
    input.close();
    
    output.open(outputname,ios::app);
    for (int i = 0; i < result.size(); i++)
    {
        output<< result[i].a + "\n";
    }
    output.close();
}
