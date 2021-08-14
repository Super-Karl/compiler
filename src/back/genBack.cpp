//
// Created by duhao on 2021/7/30.
//
#include"back.h"
#include "genback.h"
#include"../mid/ir/ir.h"
#include<vector>
#include <algorithm>
namespace compiler::back::genarm{
    vector<compiler::back::Sentence *> genBack(compiler::mid::ir::IRList ir)
    {
        vector<compiler::back::Sentence *> armList;
        for (auto val : ir)//ir的每一个块
        {
            compiler::mid::ir::GlobalData *global=dynamic_cast<compiler::mid::ir::GlobalData *>(val);
            if(global!=nullptr){
                compiler::back::Sentence *blockdeclaration=new compiler::back::BlockDeclaration(compiler::back::EQUKeywords::data);
                armList.push_back(blockdeclaration);
                string name;
                if(global->isArray==false){
                    name=global->name.erase(0,1);
                    compiler::back::Sentence *var1=new compiler::back::VarDeclaration(compiler::back::EQUKeywords::global,name);
                    armList.push_back(var1);
                    compiler::back::LABEL *label=new compiler::back::LABEL(name);
                    compiler::back::Sentence *var2=new compiler::back::Instr_Sentence(*label);
                    armList.push_back(var2);
                    compiler::back::Sentence *var3=new compiler::back::Varvalue(compiler::back::EQUKeywords::word,global->val[0]);
                    armList.push_back(var3);
                }
                if(global->isArray==true){
                    name=global->name.erase(0,2);
                    compiler::back::Sentence *var1=new compiler::back::VarDeclaration(compiler::back::EQUKeywords::global,name);
                    armList.push_back(var1);
                    compiler::back::LABEL *label=new compiler::back::LABEL(name);
                    compiler::back::Sentence *var2=new compiler::back::Instr_Sentence(*label);
                    armList.push_back(var2);
                    for(auto val:global->val){
                        compiler::back::Sentence *var3=new compiler::back::Varvalue(compiler::back::EQUKeywords::word,val);
                        armList.push_back(var3);
                    }
                }
            }
            int mainFunFlag=0;
            int sumOfValues=0;
            int funCallFlag=0;//监测是否使用函数
            int startStackSize=0;
            int curStack=0;
            int arraySize=0;
            int returnflag=0;//监测是否返回过
            vector<int> returnaddpos;
            vector<int> LDRlr;
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
                            case compiler::mid::ir::OperatorCode::Mod:{
                                auto op1=new compiler::back::OPERATION(compiler::back::Instruction::MOV32);
                                auto dest1=new compiler::back::Direct_Reg("r0");
                                auto source1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                                auto OPERAND1=new compiler::back::OPERAND(dest1,source1);
                                auto sentence1=new compiler::back::Instr_Sentence(*op1,*OPERAND1);
                                armList.push_back(sentence1);

                                auto op4=new compiler::back::OPERATION(compiler::back::Instruction::LDR);
                                auto dest4=new compiler::back::Direct_Reg("r0");
                                auto source4=new compiler::back::Indirect_Reg("r0",0);
                                auto OPERAND4=new compiler::back::OPERAND(dest4,source4);
                                auto sentence4=new compiler::back::Instr_Sentence(*op4,*OPERAND4);
                                armList.push_back(sentence4);

                                auto op2=new compiler::back::OPERATION(compiler::back::Instruction::MOV32);
                                auto dest2=new compiler::back::Direct_Reg("r1");
                                auto source2=convertVarToReg(irInstr->source2,usedReg,Regs,irInstr->source2);
                                auto OPERAND2=new compiler::back::OPERAND(dest2,source2);
                                auto sentence2=new compiler::back::Instr_Sentence(*op2,*OPERAND2);
                                armList.push_back(sentence2);

                                auto op5=new compiler::back::OPERATION(compiler::back::Instruction::LDR);
                                auto dest5=new compiler::back::Direct_Reg("r1");
                                auto source5=new compiler::back::Indirect_Reg("r1",0);
                                auto OPERAND5=new compiler::back::OPERAND(dest5,source5);
                                auto sentence5=new compiler::back::Instr_Sentence(*op5,*OPERAND5);
                                armList.push_back(sentence5);

                                auto op3=new compiler::back::OPERATION(compiler::back::Instruction::BL);
                                auto blabel3=new compiler::back::LABEL("__aeabi_idivmod");
                                auto sentence3=new compiler::back::Instr_Sentence(*op3,*blabel3);
                                armList.push_back(sentence3);

                                auto op7=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto dest7=convertVarToReg(irInstr->dest,usedReg,Regs,irInstr->dest);
                                auto source7=new compiler::back::Direct_Reg("r1");
                                auto OPERAND7=new compiler::back::OPERAND(dest7,source7);
                                auto sentence7=new compiler::back::Instr_Sentence(*op7,*OPERAND7);
                                armList.push_back(sentence7);

                                auto op6=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto dest6=new compiler::back::Direct_Reg("r0");
                                auto source6=convertVarToReg(irInstr->dest,usedReg,Regs,irInstr->dest);
                                auto OPERAND6=new compiler::back::OPERAND(dest6,source6);
                                auto sentence6=new compiler::back::Instr_Sentence(*op6,*OPERAND6);
                                armList.push_back(sentence6);
                            }
                            case compiler::mid::ir::OperatorCode::Cmp:{
                                op=new compiler::back::OPERATION(compiler::back::Instruction::CMP);
                                auto OPERAND1=convertVarToReg(irInstr->source1,usedReg,Regs,irInstr->source1);
                                auto OPERAND2=convertVarToReg(irInstr->source2,usedReg,Regs,irInstr->source2);
                                auto OPERAND=new compiler::back::OPERAND(OPERAND1,OPERAND2);
                                auto sentence=new compiler::back::Instr_Sentence(*op,*OPERAND);
                                armList.push_back(sentence);
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
                        for(auto reg:Regs){
                            stackPushBack(armList,curStack,reg);
                            curStack++;
                        }
                        curStack=0;
                        armList.push_back(sentence);//这个push的是BL
                        //函数后恢复现场
                        compiler::back::OPERATION *operationR0 = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                        compiler::back::OperateNum *dest = nullptr;
                        if(funcallInstr->retType==compiler::mid::ir::ElemType::INT){
                            //函数返回值为int
                            //cout<<allocReg(usedReg);
                            dest =  convertVarToReg(funcallInstr->retOp,usedReg,Regs,funcallInstr->retOp);
                        }
                        else{
                            dest = new compiler::back::Direct_Reg("r0");
                        }
                        compiler::back::OperateNum *sourceR0 = new compiler::back::Direct_Reg("r0");
                        compiler::back::OPERAND *operandR0=new compiler::back::OPERAND(dest,sourceR0);
                        compiler::back::Sentence  *sentenceR0=new compiler::back::Instr_Sentence(*operationR0,*operandR0);

                        compiler::back::OPERATION *operation0 = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                        compiler::back::OperateNum *dest0 = new compiler::back::Direct_Reg("r0");
                        compiler::back::OperateNum *source0 = new compiler::back::ImmNum(0);
                        compiler::back::OPERAND *operand0=new compiler::back::OPERAND(dest0,source0);
                        compiler::back::Sentence  *sentence0=new compiler::back::Instr_Sentence(*operation0,*operand0);
                        armList.push_back(sentenceR0);
                        armList.push_back(sentence0);
                        for(auto reg:Regs){
                            stackPop(armList,curStack,reg);
                            curStack++;
                        }
                        curStack=0;
                        compiler::back::OPERATION *operation1 = new compiler::back::OPERATION(compiler::back::Instruction::LDR);
                        compiler::back::OperateNum *dest1 = new compiler::back::Direct_Reg("lr");
                        compiler::back::OperateNum *source3 = new compiler::back::Indirect_Reg("sp",4);
                        compiler::back::OPERAND *operand2=new compiler::back::OPERAND(dest1,source3);
                        compiler::back::Sentence  *sentence1=new compiler::back::Instr_Sentence(*operation1,*operand2);
                        armList.push_back(sentence1);
                        LDRlr.push_back(armList.size()-1);
                    }
                    //函数返回部分
                    compiler::mid::ir::RetIR *retInstr=dynamic_cast<compiler::mid::ir::RetIR *>(funcBody);
                    if(retInstr!=nullptr){
                        if(returnflag==0) {
                            returnflag=1;
                            if (retInstr->retVal.type == compiler::mid::ir::Type::Imm) {
                                op = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto r0 = new compiler::back::Direct_Reg("r0");
                                auto Imm = new compiler::back::ImmNum(retInstr->retVal.value);
                                auto OPERAND1 = new compiler::back::OPERAND(r0, Imm);
                                auto sentence1 = new compiler::back::Instr_Sentence(*op, *OPERAND1);
                                armList.push_back(sentence1);

                                auto op2 = new compiler::back::OPERATION(compiler::back::Instruction::LDR);
                                auto r14 = new compiler::back::Direct_Reg("lr");
                                auto sp0 = new compiler::back::Indirect_Reg("sp", 0);
                                auto OPERAND2 = new compiler::back::OPERAND(r14, sp0);
                                auto sentence2 = new compiler::back::Instr_Sentence(*op2, *OPERAND2);
                                armList.push_back(sentence2);

                                auto op3 = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                                auto sp = new compiler::back::Direct_Reg("sp");
                                auto imm4 = new compiler::back::ImmNum(8);
                                auto OPERAND3 = new compiler::back::OPERAND(sp, sp, imm4);
                                auto sentence3 = new compiler::back::Instr_Sentence(*op3, *OPERAND3);
                                armList.push_back(sentence3);
                                returnaddpos.push_back(armList.size()-1);


                                auto op4 = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto pc = new compiler::back::Direct_Reg("pc");
                                auto OPERAND4 = new compiler::back::OPERAND(pc, r14);
                                auto sentence4 = new compiler::back::Instr_Sentence(*op4, *OPERAND4);
                                armList.push_back(sentence4);

                                auto op5 = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto imm0 = new compiler::back::ImmNum(0);
                                auto OPERAND5 = new compiler::back::OPERAND(r0, imm0);
                                auto sentence5 = new compiler::back::Instr_Sentence(*op5, *OPERAND5);
                                armList.push_back(sentence5);
                            }
                            else if (retInstr->retVal.type == compiler::mid::ir::Type::Var){
                                op = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto reg = new compiler::back::Direct_Reg("r0");
                                auto Imm = new compiler::back::Direct_Reg(findFromRegs(retInstr->retVal.name,usedReg));
                                auto OPERAND1 = new compiler::back::OPERAND(reg, Imm);
                                auto sentence1 = new compiler::back::Instr_Sentence(*op, *OPERAND1);
                                armList.push_back(sentence1);

                                auto op2 = new compiler::back::OPERATION(compiler::back::Instruction::LDR);
                                auto r14 = new compiler::back::Direct_Reg("lr");
                                auto sp0 = new compiler::back::Indirect_Reg("sp", 0);
                                auto OPERAND2 = new compiler::back::OPERAND(r14, sp0);
                                auto sentence2 = new compiler::back::Instr_Sentence(*op2, *OPERAND2);
                                armList.push_back(sentence2);

                                auto op3 = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                                auto sp = new compiler::back::Direct_Reg("sp");
                                auto imm4 = new compiler::back::ImmNum(4);
                                auto OPERAND3 = new compiler::back::OPERAND(sp, sp, imm4);
                                auto sentence3 = new compiler::back::Instr_Sentence(*op3, *OPERAND3);
                                armList.push_back(sentence3);
                                returnaddpos.push_back(armList.size()-1);

                                auto op4 = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto pc = new compiler::back::Direct_Reg("pc");
                                auto OPERAND4 = new compiler::back::OPERAND(pc, r14);
                                auto sentence4 = new compiler::back::Instr_Sentence(*op4, *OPERAND4);
                                armList.push_back(sentence4);

                                auto op5 = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                                auto imm0 = new compiler::back::ImmNum(0);
                                auto OPERAND5 = new compiler::back::OPERAND(reg, imm0);
                                auto sentence5 = new compiler::back::Instr_Sentence(*op5, *OPERAND5);
                                armList.push_back(sentence5);
                            }
                        }
                    }
                    //函数if导致的跳转
                    compiler::mid::ir::JmpIR *jumpInstr=dynamic_cast<compiler::mid::ir::JmpIR *>(funcBody);
                    if(jumpInstr!=nullptr){
                        returnflag=0;
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jmp){
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B);}
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jg)
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B,compiler::back::BarCode::HI);
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jle)
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B,compiler::back::BarCode::LS);
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jeq)
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B,compiler::back::BarCode::EQ);
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jl)
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B,compiler::back::BarCode::LS);
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jge)
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B,compiler::back::BarCode::GE);
                        if(jumpInstr->action==compiler::mid::ir::OperatorCode::Jne)
                            op = new compiler::back::OPERATION(compiler::back::Instruction::B,compiler::back::BarCode::NE);
                            compiler::back::LABEL *b_label=new compiler::back::LABEL(jumpInstr->label);
                            compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*op,*b_label);
                            armList.push_back(sentence);

                    }
                    compiler::mid::ir::LabelIR *labelInstr=dynamic_cast<compiler::mid::ir::LabelIR *>(funcBody);
                    if(labelInstr!=nullptr){
                        returnflag=0;
                        compiler::back::LABEL *label=new compiler::back::LABEL(labelInstr->label);
                        compiler::back::Sentence *sentence=new compiler::back::Instr_Sentence(*label);
                        armList.push_back(sentence);
                    }
                    //分配空间数组
                    compiler::mid::ir::AllocaIR *allocaIr=dynamic_cast<compiler::mid::ir::AllocaIR *>(funcBody);
                    if(allocaIr!=nullptr){
                        funCallFlag=1;
                        int size=allocaIr->size;
                        arraySize+=size;
                        auto op = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                        auto dest=new compiler::back::Direct_Reg("r0");
                        auto source1=new compiler::back::Direct_Reg("sp");
                        auto source2=new compiler::back::ImmNum(0);
                        auto operand1=new compiler::back::OPERAND(dest,source1,source2);
                        auto sentence=new compiler::back::Instr_Sentence(*op,*operand1);
                        armList.push_back(sentence);
                        auto op1 = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                        auto dest1=new compiler::back::Direct_Reg("r1");
                        auto source12=new compiler::back::ImmNum(0);
                        auto operand12=new compiler::back::OPERAND(dest1,source12);
                        auto sentence1=new compiler::back::Instr_Sentence(*op1,*operand12);
                        armList.push_back(sentence1);
                        auto op2 = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                        auto dest2=new compiler::back::Direct_Reg("r2");
                        auto source13=new compiler::back::ImmNum(4*size);
                        auto operand13=new compiler::back::OPERAND(dest2,source13);
                        auto sentence2=new compiler::back::Instr_Sentence(*op2,*operand13);
                        armList.push_back(sentence2);
                        auto op3 = new compiler::back::OPERATION(compiler::back::Instruction::BL);
                        auto b_label=new compiler::back::LABEL("memset");
                        auto sentence3=new compiler::back::Instr_Sentence(*op3,*b_label);
                        armList.push_back(sentence3);
                    }
                    auto *storeIr=dynamic_cast<compiler::mid::ir::StoreIR *>(funcBody);
                    if(storeIr!=nullptr){
                        int value=storeIr->source.value;
                        int offset=storeIr->offset.value;
                        auto op = new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                        auto dest=new compiler::back::Direct_Reg("lr");
                        auto source=new compiler::back::ImmNum(value);
                        auto operand=new compiler::back::OPERAND(dest,source);
                        auto sentence=new compiler::back::Instr_Sentence(*op,*operand);
                        armList.push_back(sentence);
                        auto op1 = new compiler::back::OPERATION(compiler::back::Instruction::STR);
                        auto dest1=new compiler::back::Direct_Reg("lr");
                        auto source1=new compiler::back::Indirect_Reg("sp",4*offset);
                        auto operand1=new compiler::back::OPERAND(dest1,source1);
                        auto sentence1=new compiler::back::Instr_Sentence(*op1,*operand1);
                        armList.push_back(sentence1);
                    }
                }



                //函数后面的部分
                if(funCallFlag==1)
                    startStackSize=4*Regs.size()+4*arraySize+4;
                else
                    startStackSize=4;
                //cout<<armList.size();

                compiler::back::OPERATION * endLDR=new compiler::back::OPERATION(compiler::back::Instruction::LDR);
                compiler::back::OperateNum * LDRPC=new compiler::back::Direct_Reg("lr");
                compiler::back::OperateNum * LDRSP=new compiler::back::Indirect_Reg("sp",startStackSize-4);
                compiler::back::OPERAND *LDROPERAND=new compiler::back::OPERAND(LDRPC,LDRSP);
                compiler::back::Sentence *LDRSentence=new compiler::back::Instr_Sentence(*endLDR,*LDROPERAND);
                armList.push_back(LDRSentence);

                compiler::back::OPERATION * endADD=new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                compiler::back::OperateNum * ADDPC=new compiler::back::Direct_Reg("sp");
                compiler::back::OperateNum * ADDLR=new compiler::back::Direct_Reg("sp");
                auto ADDImm=new compiler::back::ImmNum(startStackSize);
                compiler::back::OPERAND *endADDOPERAND=new compiler::back::OPERAND(ADDPC,ADDLR,ADDImm);
                compiler::back::Sentence *endADDSentence=new compiler::back::Instr_Sentence(*endADD,*endADDOPERAND);
                armList.push_back(endADDSentence);

                compiler::back::OPERATION * endMOV=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
                compiler::back::OperateNum * PCReg=new compiler::back::Direct_Reg("pc");
                compiler::back::OperateNum * R14Reg=new compiler::back::Direct_Reg("lr");
                compiler::back::OPERAND *endOPERAND=new compiler::back::OPERAND(PCReg,R14Reg);
                compiler::back::Sentence *endSentence=new compiler::back::Instr_Sentence(*endMOV,*endOPERAND);



                compiler::back::Instr_Sentence *beginSentence1=dynamic_cast<compiler::back::Instr_Sentence *>(armList[beginIndex]);
                compiler::back::ImmNum *stackSize=new compiler::back::ImmNum(startStackSize);//开始时栈的大小
                beginSentence1->operand.OPERAND3=stackSize;
                armList[beginIndex]=beginSentence1;
                compiler::back::Instr_Sentence *beginSentence2=dynamic_cast<compiler::back::Instr_Sentence *>(armList[beginIndex+1]);
                compiler::back::OperateNum *stackSize2=new compiler::back::Indirect_Reg("sp",startStackSize-4);
                beginSentence2->operand.OPERAND2=stackSize2;
                armList[beginIndex+1]=beginSentence2;
                //cout<<Regs.size()<<endl;
                for(auto val:LDRlr) {
                    //cout<<val<<"val"<<endl;
                    compiler::back::Instr_Sentence *LDRlrp=dynamic_cast<compiler::back::Instr_Sentence *>(armList[val]);
                    LDRlrp->operand.OPERAND2=stackSize2;
                    armList[val]=LDRlrp;
                }
                armList.push_back(endSentence);
                for(auto val:returnaddpos) {
                    auto op3 = new compiler::back::OPERATION(compiler::back::Instruction::ADD);
                    auto sp = new compiler::back::Direct_Reg("sp");
                    auto imm4 = new compiler::back::ImmNum(startStackSize);
                    auto OPERAND3 = new compiler::back::OPERAND(sp, sp, imm4);
                    auto sentence3 = new compiler::back::Instr_Sentence(*op3, *OPERAND3);
                    armList[val]=sentence3;
                }
                Regs.clear();
                usedReg.clear();
            }
        }
        return armList;
    }
    compiler::back::OperateNum *  convertVarToReg(compiler::mid::ir::OperatorName source,map<string,string> &usedReg,vector<string> &Regs,compiler::mid::ir::OperatorName source1)
    {//将操作数转换到寄存器                             ir中的操作数                           记录寄存器使用 情况的表          记录那些寄存器被使用了    记录前面的操作数

        string regName;
        compiler::back::OperateNum *armNum=nullptr;
        if(source.type==compiler::mid::ir::Type::Imm)//立即数类型
            armNum=new compiler::back::ImmNum(source.value);
        if(source.type==compiler::mid::ir::Type::Var){//寄存器类型
            if(source.name.find("%")!=-1)
                if(findFromRegs(source.name,usedReg)!="") {//表中已存在
                    armNum = new compiler::back::Direct_Reg(findFromRegs(source.name, usedReg));
                }
                else{//表中不存在,进行寄存器分配
                    string regName=allocReg(usedReg);
                    //if(Regs.size<7)
                    //    regName=allocReg(usedReg);
                    //else
                    //    pushtostack
                    if(source1.name.find("$")==-1 && findFromSecond(source1.name,usedReg)==0){
                        Regs.push_back(regName);//防止参数被加入栈中
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
        //分配寄存器
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
    bool findFromSecond(string name,map<string,string> usedReg){
        //从寄存器名寻找
        int flag=0;
        map<string,string>::iterator iter;
        for (iter=usedReg.begin(); iter!=usedReg.end(); iter++){
            //cout<<usedReg[iter->first];
            if(usedReg[iter->first] == name)//找到了
            {
                flag=1;
                break;
            }
        }
        return flag;
    }
    string findFromRegs(string name,map<string,string> usedReg)
    {
        //从变量名寻找
        auto iter=usedReg.find(name);
        if(iter != usedReg.end())
            return iter->second;
        else return "";
    }

    compiler::back::Sentence* paramToReg(compiler::mid::ir::OperatorName arg,int i,map<string,string>usedReg,vector<string> Regs)
    {
        //函数用
        //将参数转换到寄存器
        compiler::back::OPERATION *operation=new compiler::back::OPERATION(compiler::back::Instruction::MOV);
        compiler::back::OperateNum * dest=new compiler::back::Direct_Reg("r"+to_string(i));
        compiler::back::OperateNum * operatenum=convertVarToReg(arg,usedReg,Regs,arg);
        compiler::back::OPERAND * operand=new compiler::back::OPERAND(dest,operatenum);
        compiler::back::Sentence * sentence=new compiler::back::Instr_Sentence(*operation,*operand);
        return sentence;
    }
    string handleIrFunName(string irname)
    {
        //处理函数名
        irname.erase(remove(irname.begin(), irname.end(), '@'), irname.end());
        return irname;
    }
    int createStack(vector<compiler::back::Sentence *> &armList)
    {
        //建栈
        compiler::back::OPERATION *operation = new compiler::back::OPERATION(compiler::back::Instruction::SUB);
        compiler::back::OperateNum *dest = new compiler::back::Direct_Reg("sp");
        compiler::back::OperateNum *source1 = new compiler::back::Direct_Reg("sp");
        compiler::back::OperateNum *source2= new compiler::back::ImmNum(4);
        compiler::back::OPERAND *operand1=new compiler::back::OPERAND(dest,source1,source2);
        compiler::back::Sentence  *sentence=new compiler::back::Instr_Sentence(*operation,*operand1);
        armList.push_back(sentence);
        compiler::back::OPERATION *operation1 = new compiler::back::OPERATION(compiler::back::Instruction::STR);
        compiler::back::OperateNum *dest1 = new compiler::back::Direct_Reg("lr");
        compiler::back::OperateNum *source3 = new compiler::back::Indirect_Reg("sp",4);
        compiler::back::OPERAND *operand2=new compiler::back::OPERAND(dest1,source3);
        compiler::back::Sentence  *sentence1=new compiler::back::Instr_Sentence(*operation1,*operand2);
        armList.push_back(sentence1);
        return armList.size()-2;
    }
    void stackPushBack(vector<compiler::back::Sentence *> &armList,int curStack,string reg)
    {
        compiler::back::OPERATION *operation = new compiler::back::OPERATION(compiler::back::Instruction::STR);
        compiler::back::OperateNum *dest = new compiler::back::Direct_Reg(reg);
        compiler::back::OperateNum *source = new compiler::back::Indirect_Reg("sp",4*curStack);
        compiler::back::OPERAND *operand=new compiler::back::OPERAND(dest,source);
        compiler::back::Sentence  *sentence=new compiler::back::Instr_Sentence(*operation,*operand);
        armList.push_back(sentence);
    }
    void stackPop(vector<compiler::back::Sentence *> &armList,int curStack,string reg){
        compiler::back::OPERATION *operation = new compiler::back::OPERATION(compiler::back::Instruction::LDR);
        compiler::back::OperateNum *dest = new compiler::back::Direct_Reg(reg);
        compiler::back::OperateNum *source = new compiler::back::Indirect_Reg("sp",4*curStack);
        compiler::back::OPERAND *operand=new compiler::back::OPERAND(dest,source);
        compiler::back::Sentence  *sentence=new compiler::back::Instr_Sentence(*operation,*operand);
        armList.push_back(sentence);
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

}