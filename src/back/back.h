//
// Created by karl on 2021/7/29.
//

#ifndef COMPILER_BACK_H
#define COMPILER_BACK_H

#include <iostream>
#include <vector>
#include "../enum/enums.h"

using namespace std;

namespace compiler::back {

    class Source {
    public:
        SourceType Type;

        Source() = default;

        explicit Source(SourceType type) : Type(type) {}
    };


    class address : public Source {
    public:
        string addr;

        address(string reg, int offset) {
            if (offset == 0) {
                addr = "[" + reg + "]";
            } else {
                addr = "[" + reg + ",#" + to_string(offset) + "]";
            }
        }

        address(int reg, int offset) {
            if (offset == 0) {
                addr = "[r" + to_string(reg) + "]";
            } else {
                addr = "[r" + to_string(reg) + ",#" + to_string(offset) + "]";
            }
        }

        address(int reg) {
            addr = "[r" + to_string(reg) + "]";
        }

        address(string reg, int offset, string s) {
            addr = "[" + reg + ",#" + to_string(offset) + "]" + s;
        }

        address(string reg) {
            addr = reg;
        }

        address(string reg, string reg2) {
            addr = "[" + reg + "," + reg2 + "]";
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
                      ".endm\n"
                      ".arch armv7ve\n";
        }
    };

    class Lable : public INS {
    public :
        LableType lableType;
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

        Lable(string name) : INS(INSlableType) {
            fullIns = name + ":\n";
        }
    };

    class GLOBAL : public INS {
    public:
        GLOBAL(string name, int value) : INS(global) {
            fullIns = ".data\n"
                      ".global " + name + "\n"
                      + name + ":\n"
                               ".word " + to_string(value) + "\n";
        }
    };

    class GLOBALARRAY : public INS {
    public:
        GLOBALARRAY(string name, vector<int> value) : INS(globalarray) {
            fullIns = "\n.data\n"
                      ".global " + name + "\n"
                      + name + ":\n";
            int totalSize = value.size() * 4;
            int j;
            for (j = value.size() - 1; j >= 0 && value[j] == 0; j--) {}
            for (int i = 0; i <= j; i++) {
                fullIns = fullIns + ".word " + to_string(value[i]) + "\n";
            }
            fullIns = fullIns + ".space " + to_string(totalSize) + "\n";
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
                      "\tadd\tfp, sp, #4\n";
        }

        STMDB(string func) : INS(stmdb) {
            fullIns = "\tstmdb\tsp!,{r1-r6}\n";
        }
    };

    class LDMIA : public INS {
    public:
        LDMIA() : INS(ldmia) {
            fullIns = "\tsub\tsp,\tfp,\t#4\n"
                      "\tldmia\tsp!,{fp, lr}\n"
                      "\tbx\tlr\n";
        }
    };

    class BL : public INS {
    public:
        BL(string name) : INS(bl) {
            fullIns = "\tbl\t" + name + "\n";
        }
    };

    class B : public INS {
    public:
        B(string name) : INS(b) {
            fullIns = "\tb\t" + name + "\n";
        }
        B(string s,string name) : INS(b) {
            fullIns = "\tb"+s+"\t" + name + "\n";
        }
    };

    class CMPBEQ : public INS {
    public:
        CMPBEQ(string a, string b, string lable) : INS(cmpbeq) {
            fullIns = "\tcmp\t" + a + ",\t" + b + "\n"
                                                  "\tbeq\t" + lable + "\n";
        }
    };

    class CMP : public INS {
    public:
        CMP(int reg, string b) : INS(cmp) {
            fullIns = "\tcmp\tr" + to_string(reg) + ",\t" + b + "\n";
        }

        CMP(int reg1, int reg2) : INS(cmp) {
            fullIns = "\tcmp\tr" + to_string(reg1) + ",\tr" + to_string(reg2) + "\n";
        }
    };

    class MOV32 : public INS {
    public:

        MOV32(int reg, string name) : INS(mov32) {
            fullIns = "\tmov32 r" + to_string(reg) + ", " + name + "\n";
        }

        MOV32(int reg, int value) : INS(mov32) {
            fullIns = "\tmov32 r" + to_string(reg) + ", " + to_string(value) + "\n";
        }
    };

    class MOV : public INS {
    public:
        MOV(int reg, int value) : INS(mov16) {
            fullIns = "\tmov\tr" + to_string(reg) + ", #" + to_string(value) + "\n";
        }

        MOV(int reg, int value, string type) : INS(mov16) {
            if (type == "reg2reg") {
                fullIns = "\tmov\tr" + to_string(reg) + ", r" + to_string(value) + "\n";
            }
        }

        MOV(string s, int reg, int value) : INS(mov16) {
            fullIns = "\tmov" + s + "\tr" + to_string(reg) + ", #" + to_string(value) + "\n";
        }
    };

    class STR : public INS {
    public:
        STR(int reg, address addr) : INS(str) {
            fullIns = "\tstr r" + to_string(reg) + ", " + addr.addr + "\n";
        }

        STR(int reg) : INS(str) {
            fullIns = "\tstr r" + to_string(reg) + ", [sp,#-4]!\n";
        }
    };

    class LDR : public INS {
    public:
        LDR(int reg, address addr) : INS(ldr) {
            fullIns = "\tldr r" + to_string(reg) + ", " + addr.addr + "\n";
        }

        LDR(int reg, int num) : INS(ldr) {
            fullIns = "\tldr r" + to_string(reg) + ",=" + to_string(num)+"\n";
        }

        LDR(int reg, string name) : INS(ldr) {
            fullIns = "\tldr r" + to_string(reg) + ",=" + name+"\n";
        }
    };

    class MLA : public INS {
    public:
        MLA(string rd, string rm, string rs, string rn) : INS(mla) {
            fullIns = "\tmla\t" + rd + ",\t" + rm + ",\t" + rs + ",\t" + rn + "\n";
        }
        MLA(int rd, int rm, int rs, int rn) : INS(mla) {
            fullIns = "\tmla\tr" + to_string(rd) + ",\tr" + to_string(rm) + ",\tr" + to_string(rs) + ",\tr" + to_string(rn) + "\n";
        }
    };

    class OP : public INS {
    public:
        OP(string op, int rd, int rn, int rm) : INS(option) {
            fullIns = "\t" + op + " r" + to_string(rd) + ", r" + to_string(rn) + ", r" + to_string(rm) + "\n";
        }

        OP(string op, int rd, int rn, string op2) : INS(option) {
            fullIns = "\t" + op + " r" + to_string(rd) + ", r" + to_string(rn) + ", " + op2 + "\n";
        }

        OP(string op, int rd, string rn, string op2) : INS(option) {
            fullIns = "\t" + op + " r" + to_string(rd) + ", " + rn + ", " + op2 + "\n";
        }

        OP(string op, string rd, string rn, string op2) : INS(option) {
            fullIns = "\t" + op + " " + rd + ", " + rn + ", " + op2 + "\n";
        }
    };

    class PUSH : public INS {
    public:
        PUSH(initializer_list<int> list) : INS(push) {
            fullIns = "\tpush {r" + to_string(*(list.begin()));
            for (auto i = list.begin(); i != list.end(); i++) {
                fullIns = fullIns + ",r" + to_string(*i);
            }
            fullIns = fullIns + "}\n";
        }

        PUSH(string list) : INS(push) {
            fullIns = "\tpush {" + list + "}\n";
        }
    };

    class POP : public INS {
    public:
        POP(initializer_list<int> list) : INS(pop) {
            fullIns = "\tpop {r" + to_string(*(list.begin()));
            for (auto i = list.begin(); i != list.end(); i++) {
                fullIns = fullIns + ",r" + to_string(*i);
            }
            fullIns = fullIns + "}";
        }

        POP(string list) : INS(pop) {
            fullIns = "\tpop {" + list + "}\n";
        }
    };
}
#endif //COMPILER_BACK_H
