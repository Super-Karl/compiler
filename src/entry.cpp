//
// Created by wnx on 2021/6/11.
//
#include "astpass/astpass.h"
#include "controller/controller.h"
#include "front/ast/AstNode.h"
#include "back/back.h"
#include <iostream>
#include <unordered_map>

using namespace std;

using Hash = std::unordered_map<string, int>;

vector<compiler::back::Sentence *> genBack(compiler::mid::ir::IRList ir);
string convert_num(string string_ir);
compiler::back::OperateNum * convert_source(compiler::mid::ir::OperatorName source);
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
        compiler::mid::ir::FunDefIR *funCallIr=dynamic_cast<compiler::mid::ir::FunDefIR *>(val);
        if(funCallIr!= nullptr){
            compiler::back::LABEL *funNamelabel=new compiler::back::LABEL(funCallIr->name);
            compiler::back::Sentence *funName=new compiler::back::Instr_Sentence(*funNamelabel);
            armList.push_back(funName);
            for(auto funcBody:funCallIr->funcBody){
                compiler::back::OPERATION *op=nullptr;
                compiler::mid::ir::AssignIR *irInstr=dynamic_cast<compiler::mid::ir::AssignIR *>(funcBody);
                if(irInstr != nullptr) {//对指令的部分进行分解
                    switch (irInstr->operatorCode) {
                        case compiler::mid::ir::OperatorCode::Add: {
                            op = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                            string reg=convert_num(irInstr->dest.name);
                            compiler::back::OperateNum  *dest = new compiler::back::Direct_Reg(reg);
                            //转换source1
                            auto arm_num1=convert_source(irInstr->source1);
                            auto arm_num2=convert_source(irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                        }
                        case compiler::mid::ir::OperatorCode::Mov: {
                            op=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                            string reg=convert_num(irInstr->dest.name);
                            compiler::back::OperateNum  *dest = new compiler::back::Direct_Reg(reg);
                            //转换source1
                            auto arm_num1=convert_source(irInstr->source1);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                        }
                        case compiler::mid::ir::OperatorCode::Sub:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::SUB);
                            string reg=convert_num(irInstr->dest.name);
                            compiler::back::OperateNum  *dest = new compiler::back::Direct_Reg(reg);
                            //转换source1
                            auto arm_num1=convert_source(irInstr->source1);
                            auto arm_num2=convert_source(irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                        }
                        case compiler::mid::ir::OperatorCode::Mul:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::MUL);
                            string reg=convert_num(irInstr->dest.name);
                            compiler::back::OperateNum  *dest = new compiler::back::Direct_Reg(reg);
                            //转换source1
                            auto arm_num1=convert_source(irInstr->source1);
                            auto arm_num2=convert_source(irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                        }
                        case compiler::mid::ir::OperatorCode::Div:{
                            op = new compiler::back::OPERATION(compiler::back::Instruction::SDIV);
                            string reg=convert_num(irInstr->dest.name);
                            compiler::back::OperateNum  *dest = new compiler::back::Direct_Reg(reg);
                            //转换source1
                            auto arm_num1=convert_source(irInstr->source1);
                            auto arm_num2=convert_source(irInstr->source2);
                            compiler::back::OPERAND *operand = new compiler::back::OPERAND(dest,arm_num1,arm_num2);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*operand);
                            armList.push_back(sentence);
                        }
                    }
                }

            }
    }
    }
    return armList;
}
string convert_num(string string_ir)
{
    if(string_ir=="%1")return "R4";
    if(string_ir=="%2")return "R5";
    if(string_ir=="%3")return "R6";
    if(string_ir=="%4")return "R7";
    if(string_ir=="%5")return "R8";
    if(string_ir=="%6")return "R9";
    if(string_ir=="%7")return "R10";
    if(string_ir=="%8")return "R11";
}
compiler::back::OperateNum * convert_source(compiler::mid::ir::OperatorName source)
{
    compiler::back::OperateNum *arm_Num=nullptr;
    if(source.type==compiler::mid::ir::Type::Imm)
        arm_Num=new compiler::back::ImmNum(source.value);
    if(source.type==compiler::mid::ir::Type::Var)
        arm_Num=new compiler::back::Direct_Reg(convert_num(source.name));
    return arm_Num;
}
