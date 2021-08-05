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
compiler::back::Sentence* paramToReg(compiler::mid::ir::OperatorName arg,int i,map<string,string> usedReg,vector<string> Regs);
string handleIrFunName(string irname);
compiler::back::OperateNum *  convertVarToReg(compiler::mid::ir::OperatorName source,map<string,string> &usedReg,vector<string> &Regs,compiler::mid::ir::OperatorName source1);
int createStack(vector<compiler::back::Sentence *> &armList);
string findFromRegs(string name,map<string,string> usedReg);
string allocReg(map<string,string> &usedReg);
void beginFunc(string FuncName,vector<compiler::back::Sentence *> &armList);

int main(int argc, char **argv) {
  auto *argParser = new compiler::controller::ArgParser(argc, argv);

  auto *root = compiler::controller::generator::generate(argParser->input);

//  root->print();
  Hash constTbale;
  compiler::astpassir::FirstPassRoot(root, constTbale);

  auto ir = compiler::controller::generator::genIR(root);
  if (argParser->printIR)freopen("gen.ir","w",stdout);
  compiler::controller::generator::printIR(ir);
  auto arm = genBack(ir);
  for(auto var:arm){
      var->print();
  }
  return 0;
}




vector<compiler::back::Sentence *> genBack(compiler::mid::ir::IRList ir)
{
    vector<compiler::back::Sentence *> armList;
    for (auto val : ir)//ir的每一个块
    {
        int mainFunFlag=0;
        int sumOfValues=0;
        int funCallFlag=0;//监测是否使用函数
        map<string,string> usedReg;//变量和寄存器的映射关系
        vector<string> Regs;//使用过的寄存器
        int beginIndex;
        //构造开始时入栈的部分
        compiler::mid::ir::FunDefIR *funCallIr=dynamic_cast<compiler::mid::ir::FunDefIR *>(val);
        if(funCallIr!= nullptr){
            if(funCallIr->name=="main")
                mainFunFlag=1;//如果说是主函数就设置一个flag
            beginFunc(funCallIr->name,armList);
            compiler::back::LABEL *funNamelabel=new compiler::back::LABEL(handleIrFunName(funCallIr->name));
            compiler::back::Sentence *funName=new compiler::back::Instr_Sentence(*funNamelabel);
            armList.push_back(funName);
            //将函数名入栈
            beginIndex=createStack(armList);
            for(auto funcBody:funCallIr->funcBody){
                compiler::back::OPERATION *op=nullptr;
                compiler::mid::ir::AssignIR *irInstr=dynamic_cast<compiler::mid::ir::AssignIR *>(funcBody);
                if(irInstr != nullptr) {
                    //基本操作指令部分
                    switch (irInstr->operatorCode) {
                        case compiler::mid::ir::OperatorCode::Add: {
                            op = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg,Regs,irInstr->source1);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg,Regs,irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence = new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Mov: {
                            op=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                            compiler::back::OperateNum  *dest =convertVarToReg(irInstr->dest,usedReg,Regs,irInstr->source1);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1);
                            compiler::back::Sentence *sentence = new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Sub:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::SUB);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg,Regs,irInstr->dest.name);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg,Regs,irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Mul:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::MUL);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg,Regs,irInstr->source1);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg,Regs,irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                            break;
                        }
                        case compiler::mid::ir::OperatorCode::Div:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::SDIV);
                            auto dest=convertVarToReg(irInstr->dest.name,usedReg,Regs,irInstr->source1);
                            //转换source1
                            auto arm_num1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                            auto arm_num2=convertVarToReg(irInstr->source2,usedReg,Regs,irInstr->source2);
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
                    funCallFlag=1;
                    op = new compiler::back::OPERATION(compiler::back::Instruction::BL);
                    compiler::back::LABEL *jump_label=new compiler::back::LABEL(handleIrFunName(funcallInstr->funcName));
                    compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*jump_label);
                    int i=0;
                    for(auto param:funcallInstr->argList){
                        armList.push_back(paramToReg(param,i,usedReg,Regs));
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
        compiler::back::OPERATION * endMOV=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
        compiler::back::OperateNum * PCReg=new compiler::back::Direct_Reg("pc");
        compiler::back::OperateNum * R14Reg=new compiler::back::Direct_Reg("r14");
        compiler::back::OPERAND *endOPERAND=new compiler::back::OPERAND(PCReg,R14Reg);
        compiler::back::Sentence *endSentence=new compiler::back::Instr_Sentence(*endMOV,*endOPERAND);
        //函数后面的部分
        compiler::back::Instr_Sentence *beginSentence1=dynamic_cast<compiler::back::Instr_Sentence *>(armList[beginIndex]);
        compiler::back::ImmNum *stackSize=new compiler::back::ImmNum(4*Regs.size());//开始时栈的大小
        beginSentence1->operand.OPERAND3=stackSize;
        armList[beginIndex]=beginSentence1;
        compiler::back::Instr_Sentence *beginSentence2=dynamic_cast<compiler::back::Instr_Sentence *>(armList[beginIndex+1]);
        compiler::back::OperateNum *stackSize2=new compiler::back::Indirect_Reg("sp",4*Regs.size()-4);
        beginSentence2->operand.OPERAND2=stackSize2;
        cout<<Regs.size()<<endl;
        armList[beginIndex+1]=beginSentence2;
        armList.push_back(endSentence);
        Regs.clear();
        usedReg.clear();
    }
    return armList;
}
compiler::back::OperateNum *  convertVarToReg(compiler::mid::ir::OperatorName source,map<string,string> &usedReg,vector<string> &Regs,compiler::mid::ir::OperatorName source1)
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
                 string regName=allocReg(usedReg);
                 if(source1.name.find("$")==-1){
                     Regs.push_back(regName);
                     cout<<"paramter deleted"<<endl;
                 }
                 armNum=new compiler::back::Direct_Reg(regName);
                 usedReg.insert(map<string, string>::value_type(source.name, regName));
            }
         else if(source.name.find("$")!=-1){
             string name;
             if(source.name=="$0")name="r0";
             if(source.name=="$1")name="r1";
             if(source.name=="$2")name="r2";
             if(source.name=="$3")name="r3";
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
      string regName="r"+ to_string(regNum);
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

compiler::back::Sentence* paramToReg(compiler::mid::ir::OperatorName arg,int i,map<string,string>usedReg,vector<string> Regs)
{
    compiler::back::OPERATION *operation=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
    compiler::back::OperateNum * dest=new compiler::back::Direct_Reg("r"+to_string(i));
    compiler::back::OperateNum * operatenum=convertVarToReg(arg,usedReg,Regs,arg);
    compiler::back::OPERAND * operand=new compiler::back::OPERAND(dest,operatenum);
    compiler::back::Sentence * sentence=new compiler::back::Instr_Sentence(*operation,*operand);
    return sentence;
}
string handleIrFunName(string irname)
{
    irname.erase(remove(irname.begin(), irname.end(), '@'), irname.end());
    return irname;
}
int createStack(vector<compiler::back::Sentence *> &armList)
{
    compiler::back::OPERATION *operation = new compiler::back::OPERATION(compiler::back::Instruction::SUB);
    compiler::back::OperateNum *dest = new compiler::back::Direct_Reg("sp");
    compiler::back::OperateNum *source1 = new compiler::back::Direct_Reg("sp");
    compiler::back::OperateNum *source2= new compiler::back::ImmNum(4);
    compiler::back::OPERAND *operand1=new compiler::back::OPERAND(dest,source1,source2);
    compiler::back::Sentence  *sentence=new compiler::back::Instr_Sentence(*operation,*operand1);
    armList.push_back(sentence);
    compiler::back::OPERATION *operation1 = new compiler::back::OPERATION(compiler::back::Instruction::STR);
    compiler::back::OperateNum *dest1 = new compiler::back::Direct_Reg("r14");
    compiler::back::OperateNum *source3 = new compiler::back::Indirect_Reg("sp",4);
    compiler::back::OPERAND *operand2=new compiler::back::OPERAND(dest1,source3);
    compiler::back::Sentence  *sentence1=new compiler::back::Instr_Sentence(*operation1,*operand2);
    armList.push_back(sentence1);
    return armList.size()-2;
}
void stackPushBack(vector<compiler::back::Sentence *> &armList)
{

}
void beginFunc(string FuncName,vector<compiler::back::Sentence *> &armList)
{
    compiler::back::Sentence *blockdeclaration=new compiler::back::BlockDeclaration(compiler::back::EQUKeywords::text);
    armList.push_back(blockdeclaration);
    compiler::back::LABEL *funcName=new compiler::back::LABEL(FuncName);
    compiler::back::Sentence *declare1=new compiler::back::TypeDeclaration(compiler::back::EQUKeywords::global,*funcName,compiler::back::TYPE::NOP);
    compiler::back::Sentence *declare2=new compiler::back::TypeDeclaration(compiler::back::EQUKeywords::type,*funcName,compiler::back::TYPE::function);
    armList.push_back(declare1);
    armList.push_back(declare2);
}