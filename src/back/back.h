//
// Created by karl on 2021/7/29.
//

#ifndef COMPILER_BACK_H
#define COMPILER_BACK_H

#include <iostream>
#include "../enum/enums.h"

using namespace std;

namespace compiler::back {


    class Source {
    public:
        SourceType Type;

        Source() = default;

        explicit Source(SourceType type) : Type(type) {}
    };

    class VarSource : public Source {
    public:
        static int count;
        string name;

        explicit VarSource(SourceType type = VarSourceType) : Source(type) {
            name = "%" + to_string(count);
            count++;
        }

        explicit VarSource(string realname, SourceType type = VarSourceType) : Source(type) {
            name = "%" + realname + to_string(count);
            count++;
        }
    };

    class NumSource : public Source {
    public:
        int num;

        explicit NumSource(int num, SourceType type = NumSourceType) : Source(type), num(num) {}
    };

    class address :public Source{
    public:
        string addr;
        address(string reg,int offset){
            if(offset==0)
            {
                addr="["+reg+"]";
            }
            else
            {
                addr="["+reg+",#"+to_string(offset)+"]";
            }
        }
        address(string reg,int offset,string s){
            addr="["+reg+",#"+to_string(offset)+"]"+s;
        }
        address(string reg){
            addr=reg;
        }
    };

    //指令
    class INS {
    public:
        INSType insType;
        string fullIns;

        INS(INSType insType1) : insType(insType1) {}

        void print() { cout << fullIns; }

        string getFullIns() { return fullIns; }
    };//基类

    class MACRO : public INS {
    public:
        MACRO() : INS(macroType) {
            fullIns = ".macro\tmov32,\treg,\tval\n"
                      "    movw\t\\reg,\t#:lower16:\\val\n"
                      "    movt\t\\reg,\t#:upper16:\\val\n"
                      ".endm\n";
        }
    };

    class Lable : public INS {
    public :
        LableType lableType;
        static int FunctionCount;
        static int IfCount;
        string name;

        Lable(LableType lableType, string realName = "NULL") : lableType(lableType), INS(INSlableType) {
            switch (lableType) {
                case FunctionType: {
                    name = "function_" + realName;// + "_" + to_string(FunctionCount);
                    fullIns = name;
                    break;
                }
            }
        }
    };

    class GLOBAL:public INS{
    public:
        GLOBAL(string name,int value):INS(global)
        {
            fullIns=".data\n"
                    ".global gv_"+name+"\n"
                                       "gv_"+name+":\n"
                    ".word "+to_string(value)+"\n";
        }
    };

    class FUNC : public INS {
    public:
        string name;

        FUNC(string name) : name(name), INS(funcLable) {
            fullIns = ".text\n"
                      ".global\t" + name + "\n"
                                           ".type\t" + name + ",\t%function\n" +
                      name + ":\n";
        }
    };

    class STMDB : public INS {
    public:
        STMDB() : INS(stmdb) {
            fullIns = "\tstmdb\tsp!,{fp, lr}\n"
                      "\tadd\tfp,sp,#4\n";
        }
    };

    class LDMIA : public INS {
    public:
        LDMIA() : INS(ldmia) {
            fullIns = "\tldmia\tsp!,{fp, lr}\n"
                      "\tbx\tlr\n";
        }
    };

    class MOV32 : public INS {
    public:

        MOV32(int reg,string name) : INS(mov32) {
            fullIns = "\tmov32 r"+to_string(reg)+", "+name+"\n";
        }
        MOV32(int reg,int value) : INS(mov32) {
            fullIns = "\tmov32 r"+to_string(reg)+", "+to_string(value)+"\n";
        }
    };

    class MOV : public INS {
    public:
        MOV(int reg, int value) : INS(mov16) {
            fullIns = "\tmov\tr" + to_string(reg) + ",#" + to_string(value) + "\n";
        }

        MOV(int reg, int value, string type) : INS(mov16) {
            if (type == "reg2reg") {
                fullIns = "\tmov\tr" + to_string(reg) + ",r" + to_string(value) + "\n";
            }
        }
    };

    class STR:public INS{
    public:
        STR(int reg,address addr):INS(str){
            fullIns = "\tstr r"+to_string(reg)+", "+addr.addr+"\n";
        }
    };

    class LDR:public INS{
    public:
        LDR(int reg,address addr):INS(ldr){
            fullIns = "\tldr r"+to_string(reg)+", "+addr.addr+"\n";
        }
    };
}
#endif //COMPILER_BACK_H
