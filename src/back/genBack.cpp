//
// Created by duhao on 2021/7/30.
//
#include"back.h"
#include"../mid/ir/ir.h"
#include<vector>
namespace compiler::back::genarm{
    void genarm(compiler::mid::ir::IR * ir)
    {
        compiler::mid::ir::FunCallIR *funCallIr=dynamic_cast<compiler::mid::ir::FunCallIR *>(ir);
        if(funCallIr!= nullptr)
            std::cout<<1;
    }

}