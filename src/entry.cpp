//
// Created by wnx on 2021/6/11.
//
#include "astpass/astpass.h"
#include "controller/controller.h"
#include "front/ast/AstNode.h"
#include "back/back.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

using Hash = std::unordered_map<string, int>;

vector<compiler::back::Sentence *> genBack(compiler::mid::ir::IRList ir);
compiler::back::Sentence* paramToReg(compiler::mid::ir::OperatorName arg,int i,map<string,string> usedReg);
string handleIrFunName(string irname);
compiler::back::OperateNum *  convertVarToReg(compiler::mid::ir::OperatorName source,map<string,string> &usedReg);
void create_stack(vector<compiler::back::Sentence *> &armList);
string findFromRegs(string name,map<string,string> usedReg);
string allocReg(map<string,string> &usedReg);
int main(int argc, char **argv) {
  auto *argParser = new compiler::controller::ArgParser(argc, argv);

  auto *root = compiler::controller::generator::generate(argParser->input);

  root->print();
  Hash constTbale;
  compiler::astpassir::FirstPassRoot(root, constTbale);

  auto ir = compiler::controller::generator::genIR(root);
  compiler::controller::generator::printIR(ir);
  auto arm = genBack(ir);
  return 0;
}
vector<compiler::back::Sentence *> genBack(compiler::mid::ir::IRList ir)
{
    vector<compiler::back::Sentence *> armList;
    for (auto val : ir)//ir的每一个块
    {
        int mainFunFlag=0;
        map<string,string> usedReg;
        //构造开始时入栈的部分
        compiler::mid::ir::FunDefIR *funCallIr=dynamic_cast<compiler::mid::ir::FunDefIR *>(val);
        if(funCallIr!= nullptr){
            if(funCallIr->name=="main")
                mainFunFlag=1;//如果说是主函数就设置一个flag
            compiler::back::LABEL *funNamelabel=new compiler::back::LABEL(handleIrFunName(funCallIr->name));
            compiler::back::Sentence *funName=new compiler::back::Instr_Sentence(*funNamelabel);
            armList.push_back(funName);
            create_stack(armList);
            for(auto funcBody:funCallIr->funcBody){
                compiler::back::OPERATION *op=nullptr;
                compiler::mid::ir::AssignIR *irInstr=dynamic_cast<compiler::mid::ir::AssignIR *>(funcBody);
                if(irInstr != nullptr) {
                    //基本操作指令部分
                    switch (irInstr->operatorCode) {
                        case compiler::mid::ir::OperatorCode::Add: {
                            op = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                            auto dest=convertVarToReg(irInstr->dest,usedReg);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence = new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Mov: {
                            op=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                            compiler::back::OperateNum  *dest =convertVarToReg(irInstr->dest,usedReg);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1);
                            compiler::back::Sentence *sentence = new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Sub:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::SUB);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Mul:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::MUL);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Div:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::SDIV);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }

                    }
                }
                //函数调用部分
                compiler::mid::ir::FunCallIR *funcallInstr=dynamic_cast<compiler::mid::ir::FunCallIR *>(funcBody);
                if(funcallInstr!=nullptr){
                    op = new compiler::back::OPERATION(compiler::back::Instruction::BL);
                    compiler::back::LABEL *jump_label=new compiler::back::LABEL(handleIrFunName(funcallInstr->funcName));
                    compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*jump_label);
                    int i=0;
                    for(auto param:funcallInstr->argList){
                        armList.push_back(paramToReg(param,i,usedReg));
                        i++;
                    }
                    armList.push_back(sentence);
                }
                //函数返回部分
                compiler::mid::ir::RetIR *retInstr=dynamic_cast<compiler::mid::ir::RetIR *>(funcBody);
                if(funcallInstr!=nullptr){
                    op = new compiler::back::OPERATION(compiler::back::Instruction::BL);
                }

            }
        }
    }
    return armList;
}
compiler::back::OperateNum *  convertVarToReg(compiler::mid::ir::OperatorName source,map<string,string> &usedReg)
{
    string regName;
    compiler::back::OperateNum *armNum=nullptr;
    if(source.type==compiler::mid::ir::Type::Imm)//立即数类型
        armNum=new compiler::back::ImmNum(source.value);
    if(source.type==compiler::mid::ir::Type::Var){//寄存器类型
        if(source.name.find("%")!=-1)
            if(findFromRegs(source.name,usedReg)!="") {//表中已存在
                //cout << source.name << "exists" << endl;
                armNum = new compiler::back::Direct_Reg(findFromRegs(source.name, usedReg));
            }
            else{//表中不存在,进行寄存器分配
                 //cout<<source.name<<"created"<<endl;
                 string regName=allocReg(usedReg);
                 //cout<<regName<<"regName"<<endl;
                 armNum=new compiler::back::Direct_Reg(regName);
                 usedReg.insert(map<string, string>::value_type(source.name, regName));
            }
         else if(source.name.find("$")!=-1){
             string name;
             if(source.name=="$0")name="R0";
             if(source.name=="$1")name="R1";
             if(source.name=="$2")name="R2";
             if(source.name=="$3")name="R3";
             armNum=new compiler::back::Direct_Reg(name);
         }
         else if(source.name.find("@")!=-1){}
    }
    return armNum;
}
string allocReg(map<string,string> &usedReg)
{
    int regNum=4;
    for(int i=0;i<7;i++)
    {
      int flag=0;
      map<string,string>::iterator iter;
      string regName="R"+ to_string(regNum);
      for (iter=usedReg.begin(); iter!=usedReg.end(); iter++){
        //cout<<usedReg[iter->first];
        if(usedReg[iter->first] == regName)//找到了
        {
          flag=1;
        }
      }
        if(flag==1)regNum++;
        else
            return regName;
    }
}

string findFromRegs(string name,map<string,string> usedReg)
{
    auto iter=usedReg.find(name);
    if(iter != usedReg.end())
        return iter->second;
    else return "";
}

compiler::back::Sentence* paramToReg(compiler::mid::ir::OperatorName arg,int i,map<string,string>usedReg)
{
    compiler::back::OPERATION *operation=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
    compiler::back::OperateNum * dest=new compiler::back::Direct_Reg("R"+to_string(i));
    compiler::back::OperateNum * operatenum=convertVarToReg(arg,usedReg);
    compiler::back::OPERAND * operand=new compiler::back::OPERAND(dest,operatenum);
    compiler::back::Sentence * sentence=new compiler::back::Instr_Sentence(*operation,*operand);
    return sentence;
}
string handleIrFunName(string irname)
{
    irname.erase(remove(irname.begin(), irname.end(), '@'), irname.end());
    return irname;
}
void create_stack(vector<compiler::back::Sentence *> &armList)
{
    compiler::back::OPERATION *operation = new compiler::back::OPERATION(compiler::back::Instruction::STM,compiler::back::stmType::DB);
    compiler::back::OperateNum *dest = new compiler::back::Direct_Reg("sp",compiler::back::Suffix::e_mark);
    vector<compiler::back::Direct_Reg> regs;
    regs.push_back(compiler::back::Direct_Reg("fp"));
    regs.push_back(compiler::back::Direct_Reg("lr"));
    compiler::back::OperateNum *regList=new compiler::back::NumList(regs);
    compiler::back::OPERAND *operand1=new compiler::back::OPERAND(dest,regList);
    compiler::back::Sentence *sentence1=new compiler::back::Instr_Sentence(*operation,*operand1);
    armList.push_back(sentence1);
    compiler::back::OPERATION *operation1 = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
    compiler::back::OperateNum *dest1 = new compiler::back::Direct_Reg("fp");
    compiler::back::OperateNum *source1 = new compiler::back::Direct_Reg("sp");
    compiler::back::OperateNum *source2=new compiler::back::ImmNum(4);
    compiler::back::OPERAND *operand2=new compiler::back::OPERAND(dest1,source1,source2);
    compiler::back::Sentence *sentence2=new compiler::back::Instr_Sentence(*operation,*operand2);
    armList.push_back(sentence2);
}
void stackPushBack(vector<compiler::back::Sentence *> &armList)
{

}


