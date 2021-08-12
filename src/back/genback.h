//
// Created by duhao on 2021/8/11.
//
#include <string>
#include "../mid/ir/ir.h"
#include <vector>
#include <map>
#include "back.h"
using namespace std;
#ifndef COMPILER_GENBACK_H
namespace compiler::back::genarm{
    vector<compiler::back::Sentence *> genBack(compiler::mid::ir::IRList ir);

    compiler::back::Sentence* paramToReg(compiler::mid::ir::OperatorName arg,int i,map<string,string> usedReg,vector<string> Regs);

    string handleIrFunName(string irname);

    compiler::back::OperateNum *  convertVarToReg(compiler::mid::ir::OperatorName source,map<string,string> &usedReg,vector<string> &Regs,compiler::mid::ir::OperatorName source1);

    int createStack(vector<compiler::back::Sentence *> &armList);

    string findFromRegs(string name,map<string,string> usedReg);

    string allocReg(map<string,string> &usedReg);

    bool findFromSecond(string name,map<string,string> usedReg);

    void beginFunc(string FuncName,vector<compiler::back::Sentence *> &armList);

    void stackPushBack(vector<compiler::back::Sentence *> &armList,int curStack,string reg);

    void stackPop(vector<compiler::back::Sentence *> &armList,int curStack,string reg);

}
#define COMPILER_GENBACK_H

#endif //COMPILER_GENBACK_H
