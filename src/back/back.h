//
<<<<<<< HEAD
// Created by duhao on 2021/7/14.
//目前只涉及ARM的部分,没有涉及Thumb指令
//在本段代码中,程序只是进行 了转换,并没有理解 做了什么,所以并不需要做包括检查CPSR等操作.
//所以目前感觉没有必要设置CPSR的类,因为并没有在指令中显性说明,所以只是翻译句子的话没有必要写CPSR
=======
// Created by karl on 2021/7/29.
>>>>>>> optimize
//

#ifndef COMPILER_BACK_H
#define COMPILER_BACK_H
<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
namespace compiler::back{
    //TODO:并没有加汇编语言程序中表达式和运算符的部分(因为我使用的时候一直在报错,所以并不明白他干了什么)
    class LABEL;//标号域

    class OPERATION;//操作助记符域

    class OPERAND;//操作数域

    class COMMENT;//注释域,不会用到注释

    class Sentence;//一条语句

    class CodeBlock;//一个代码段

    //TODO 这个类还没有实现,目前感觉到句后直接往vector里面塞就好.

    using CodeBlocks=std::vector<Sentence *>;

    enum class Instruction{
        //指令集
        //虽然大写和小写都可以,但是转换的时候均以大写为主
        //虽然有些指令目前用不到,但是还是直接加进去了
        NOP,
        ADC,
        ADD,
        AND,
        B,
        BIC,
        BL,
        BLX,
        BX,
        CDP,
        CMN,
        CMP,
        EOR,
        LDC,
        LDM,
        LDR,
        MCR,
        MOV,
        MRC,
        MRS,
        MSR,
        MUL,
        MLA,
        MVN,
        ORR,
        RSB,
        RSC,
        SBC,
        STC,
        STM,
        STR,
        SUB,
        SWI,
        SWP,
        SDIV,//说是除法(?)
        TEQ,
        TST,
        MOV32
    };//TODO 加上除法(没有除法?)
    std::string printInstruction(Instruction instr);
    enum class BarCode{
        //生成的条件码
        NOP,
        EQ,
        NE,
        CS,
        CC,
        MI,
        PL,
        VS,
        VC,
        HI,
        LS,
        GE,
        LT,
        GT,
        LE,
        AL,
    };
    std::string printBarcode(BarCode barCode);
    enum class Suffix{
        //指令的后缀,只有!和 S两种
        SNOP,
        S,
        e_mark//指!
    };
    //加点或符号会标错,所以用其他符号先代替
    std::string printSuffix(Suffix suffix);
    enum class EQUKeywords{
        //定义伪操作
       nop,
       byte,
       hword,
       Eshort,
       word,
       Eint,
       quad,
       Efloat,
       Edouble,
       ascii,
       asciz,
       zero,
       space,
       code32,
       code16,
       fpu,
       section,
       text,
       data,
       bss,
       align,
       type,
       size,
       org,
       global,
       include,
       end,
       _start,
       macro,
       endm,
       exitm,
       ifdef,
       Eelse,
       endif,
       rept,
       equ,
       list,
       nolist,
       req,
       unreq,
       pool
    };
    std::string printEQUKeywords(EQUKeywords equKeywords);
    //前面应该加上%
    enum class TYPE{
        NOP,
        object,
        function
    };
    std::string printType(TYPE type);
    enum class stmType{
        NOP,
        IA,
        IB,
        DA,
        DB,
        FD,
        ED,
        FA,
        EA
    };
    std::string printstmType(stmType stmtype);
    //现在是标号域
    class LABEL{
    public:
        std::string LabelName;
        LABEL(std::string name):LabelName(name){};
        LABEL(){};
        std::string print(){
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<LabelName;
            //outfile.close();
            return LabelName;
        }
    };

    //操作助记符域
    class OPERATION{
        //操作助记符
    public:
        Instruction instruction;
        BarCode barcode;
        stmType stmtype;
        OPERATION(Instruction instr,BarCode bcode):instruction(instr),barcode(bcode){};
        OPERATION(Instruction instr):instruction(instr){};
        OPERATION(Instruction instruction1,stmType stmtype1):stmtype(stmtype1),instruction(instruction1){};
        OPERATION(){};
        std::string print(){
            std::string word;
            word+=printInstruction(instruction);
            word+=printBarcode(barcode);
            word+=printstmType(stmtype);
            return word;
        }
    };

    //操作数域
    //寄存器后面可能会跟一个如LSL#1,代表左移三位,所以需要定义一个类来作为这个操作
    class OffsetOperate{
        public:
            Instruction OffsetInstr;
            int OffsetNum=0;
            OffsetOperate(){};
            OffsetOperate(Instruction instr,int num):OffsetInstr(instr),OffsetNum(num){};
            std::string print(){
                std::string word;
                word+=printInstruction(OffsetInstr);
                //std::fstream //outfile;
                //outfile.open("testcase.s", std::ios::app);
                if(OffsetNum!=0){
                //outfile<<" #"<<OffsetNum;
                word+=(" #"+std::to_string(OffsetNum));
                }
                //outfile.close();
                return word;
            }
    };
    //操作数类
    class OperateNum{
    public:
    OperateNum(){};
     virtual OperateNum *getThis(){
         return this;
     }
     virtual std::string print(){};
    };
    //定义操作数,包括变量,常量,寄存器,表达式
    //定义寄存器类(包括直接访问寄存器 R0,间接访问寄存器[R0])
    class Direct_Reg:public OperateNum{
    public:
        std::string RegName;
        int RegNum;
        Suffix suffix;
        OffsetOperate offsetoperate;
        //TODO 完成这个类的构造,并不确定是否需要这个
        Direct_Reg(std::string name,Instruction instr,int num);
        Direct_Reg(std::string name, Suffix suf){};
        Direct_Reg(std::string name):RegName(name){};
        Direct_Reg *getThis() override {
            return this;
        }
        std::string print(){
            std::string word="";
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            if(RegName!=""){
                //outfile<<RegName;
                word+=RegName;
                word+=printSuffix(suffix);
                word+=offsetoperate.print();
            }
            //outfile.close();
            return word;
        }

    };
    class Indirect_Reg:public OperateNum{
    public:
        std::string RegName;
        int RegNum;
        int Offset=0;
        //TODO 完成这两个类的构造,并不确定是否需要这个
        Indirect_Reg(std::string name,int Offset1=0):RegName(name),Offset(Offset1){};
        Indirect_Reg(int num,int Offset=0);
        std::string print(){
            std::string word;
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            if(RegName!=""){
                //outfile<<"["<<RegName;
                word+=("["+RegName);
            }
                //outfile<<",#"<<Offset;
                word+=(",#"+std::to_string(Offset));
            //outfile<<"]";
            word+="]";
            //outfile.close();
            return word;
        }
    };

    class ImmNum:public OperateNum{
    public:
        int value;
        ImmNum(int num):value(num){};
        ImmNum *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<"#"<<value;
            word+=("#"+std::to_string(value));
            //outfile.close();
            return word;
        }

    };

    class ConstNum:public OperateNum{
    public:
        int value;
        ConstNum(int num):value(num){};
        ConstNum *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<value;
            word+=value;
            //outfile.close();
            return word;
        }
    };
    class NumList:public OperateNum{
    public:
        std::vector<Direct_Reg> regs;
        NumList(std::vector<Direct_Reg> regs1):regs(regs1){};
        std::string  print(){
            std::string word;
            for(auto val:regs){
                word+=val.print();
                return word;
            }
        }
    };
//操作数域
    class OPERAND{
    public:
        //TODO 最多只有三个操作数(?)
        OperateNum *OPERAND1=nullptr;//这个只能是寄存器类型,为了统一,所以直接使用指针
        OperateNum *OPERAND2=nullptr;
        OperateNum *OPERAND3=nullptr;
        //OPERAND(){};
        OPERAND(OperateNum *operand1 = nullptr,OperateNum *operand2 = nullptr,OperateNum *operand3= nullptr):OPERAND1(operand1),OPERAND2(operand2),OPERAND3(operand3){};
        std::string print(){
            std::string word;
            if(OPERAND1!= nullptr)
                word+=OPERAND1->print();
            if(OPERAND2!= nullptr){
                //std::fstream //outfile;
                //outfile.open("testcase.s", std::ios::app);
                //outfile<<",";
                //outfile.close();
                word+=",";
                //std::cout<<word;
                word+=OPERAND2->print();
            }
            if(OPERAND3!= nullptr){
                //std::fstream //outfile;
                //outfile.open("testcase.s", std::ios::app);
                //outfile<<",";
                //outfile.close();
                word+=",";
                word+=OPERAND3->print();
            }
            return word;
        }
    };
//三大部分构造完毕,注释域不用管
//将三大部分整合成一个语句
    class Sentence{
    public:
        Sentence(){};
        virtual Sentence  *getThis(){
            return this;
        }
        virtual std::string print(){};
    };
    //正常指令的类型
    class Instr_Sentence:public Sentence{
    public:
        LABEL label;
        OPERATION operation;
        OPERAND operand;
        LABEL b_label;//跳转的话是直接LABEL
        Instr_Sentence()=default;
        Instr_Sentence(LABEL label1,OPERATION operation1,OPERAND operand1): label(label1), operation(operation1),operand(operand1){};
        Instr_Sentence(OPERATION operation1,OPERAND operand1): operation(operation1), operand(operand1){};
        Instr_Sentence(LABEL label1):label(label1){};
        Instr_Sentence (LABEL label1,OPERATION operation1,LABEL b_label1): label(label1), operation(operation1),
                                                                           b_label(b_label1){};
        Instr_Sentence (OPERATION operation1,LABEL b_label1):operation(operation1),b_label(b_label1){};
        Instr_Sentence(OPERATION operation1): operation(operation1){};//NOP
        Instr_Sentence *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            word+=label.print();
            if(label.LabelName!=""){
                //std::fstream //outfile;
                //outfile.open("testcase.s", std::ios::app);
                //outfile<<": ";
                //outfile.close();
                word+=": ";
            }
            else{
                //std::fstream //outfile;
                //outfile.open("testcase.s", std::ios::app);
                //outfile<<"    ";
                //outfile.close();
                word+="    ";
            }
            word+=operation.print();
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<" ";
            //outfile.close();
            word+=" ";
            word+=operand.print();
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<" ";
            //outfile.close();
            word+=" ";
            word+=b_label.print();
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<std::endl;
            //outfile.close();
            word+='\n';
            return word;
        }
    };
    //伪指令类型(因为伪指令的形式比较多,想写在一起,也有合并的可能,但是目前先写开)
    //代码块类型声明
    class BlockDeclaration:public  Sentence{
    public:
        EQUKeywords equkeywords;
        BlockDeclaration(EQUKeywords equkeywords1):equkeywords(equkeywords1){};
        BlockDeclaration *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            word+=printEQUKeywords(equkeywords);
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            word+='\n';
            //outfile<<std::endl;
            //outfile.close();
            return word;
        }
    };
    //变量类型声明(全局/局部)
    class VarDeclaration:public Sentence{
    public:
        EQUKeywords equKeywords;
        LABEL label;
        VarDeclaration( EQUKeywords equKeywords1,std::string name1=""):equKeywords(equKeywords1),label(name1){};
        VarDeclaration *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            word+=printEQUKeywords(equKeywords);
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<" ";
            //outfile.close();
            word+=" ";
            word+=label.print();
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<std::endl;
            //outfile.close();
            word+='\n';
            //outfile.close();
            return word;
        }
    };
    class Varvalue:public Sentence{
    public:
        EQUKeywords equKeywords;
        int value;
        Varvalue(EQUKeywords equKeywords1,int value1):equKeywords(equKeywords1),value(value1){};
        Varvalue *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            word+=printEQUKeywords(equKeywords);
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<" "<<value;
            word+=(" "+std::to_string(value));
            //outfile<<std::endl;
            word+='\n';
            //outfile.close();
            return word;
        }
    };
    class TypeDeclaration:public  Sentence{
    public:
        EQUKeywords equkeywords;
        LABEL label;
        TYPE type=TYPE::NOP;
        TypeDeclaration(EQUKeywords equkeywords1,LABEL label1,TYPE type1):equkeywords(equkeywords1), label(label1),type(type1){};
        TypeDeclaration(EQUKeywords equkeywords1,LABEL label1):equkeywords(equkeywords1),label(label1){};
        TypeDeclaration *getThis() override{
            return this;
        }
        std::string print(){
            std::string word;
            word+=printEQUKeywords(equkeywords);
            //std::fstream //outfile;
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<" ";
            //outfile.close();
            word+=" ";
            word+=label.print();
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<" ";
            //outfile.close();
            word+=" ";
            word+=printType(type);
            //outfile.open("testcase.s", std::ios::app);
            //outfile<<std::endl;
            //outfile.close();
            word+='\n';
            return word;
=======

#include <iostream>
#include <vector>
#include "../enum/enums.h"

using namespace std;

namespace compiler::back {

    class Source {
    public:
        SourceType Type;

        Source() = default;
    };


    class address : public Source {
    public:
        string addr;

        address(string reg, int reg1, string s) {
            addr = "[" + reg + "," + s + "r" + to_string(reg1) + "]";
        }

        address(string reg, int reg1) {
            addr = "[" + reg + ","  + to_string(reg1) + "]";
        }

        address(int reg, int offset) {
            if (offset == 0) {
                addr = "[r" + to_string(reg) + "]";
            } else {
                addr = "[r" + to_string(reg) + ",#" + to_string(offset) + "]";
            }
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

        FUNC(string name) : INS(funcLable) {
            fullIns = ".text\n"
                      ".global\t" + name + "\n"
                                           ".type\t" + name + ",\t%function\n" +
                      name + ":\n";
            if(name=="main"){
                fullIns = fullIns+"\tmov32 r0, 10000\n"
                                  "\tmov r12, sp\n"
                                  "\tsub sp, sp, r0\n";
            }
        }
    };

    class STMDB : public INS {
    public:
        STMDB() : INS(stmdb) {
            fullIns = "\tstmdb\tsp!,{fp, lr}\n"
                      "\tadd\tfp, sp, #4\n";
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

        B(string s, string name) : INS(b) {
            fullIns = "\tb" + s + "\t" + name + "\n";
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
    };

    class MOV : public INS {
    public:

        MOV(int reg, int reg2, string type) : INS(mov16) {
            if (type == "reg2reg") {
                fullIns = "\tmov\tr" + to_string(reg) + ", r" + to_string(reg2) + "\n";
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
            //fullIns = "\tldr r" + to_string(reg) + ",=" + to_string(num) + "\n";
            if (num >= 0 && num < 65536)
                fullIns = "\tmov r" + to_string(reg) + ", #" + to_string(num) + "\n";
            else
                fullIns = "\tmov32 r" + to_string(reg) + ", " + to_string(num) + "\n";
        }
    };

    class MLA : public INS {
    public:
        MLA(int rd, int rm, int rs, int rn) : INS(mla) {
            fullIns = "\tmla\tr" + to_string(rd) + ",\tr" + to_string(rm) + ",\tr" + to_string(rs) + ",\tr" + to_string(rn) + "\n";
        }
    };

    class OP : public INS {
    public:
        OP(string op, int rd, int rn, int rm) : INS(option) {
            fullIns = "\t" + op + " r" + to_string(rd) + ", r" + to_string(rn) + ", r" + to_string(rm) + "\n";
        }

        OP(string op, int rd, string rn, int rm) : INS(option) {
            fullIns = "\t" + op + " r" + to_string(rd) + ", " + rn + ", r" + to_string(rm) + "\n";
        }

        OP(string op, int rd, int rn, string op2) : INS(option) {
            fullIns = "\t" + op + " r" + to_string(rd) + ", r" + to_string(rn) + ", " + op2 + "\n";
        }

        OP(string op, string rd, string rn, int reg) : INS(option) {
            fullIns = "\t" + op + " " + rd + ", " + rn + ", r" + to_string(reg) + "\n";
        }
    };

    class PUSH : public INS {
    public:
        /*PUSH(initializer_list<int> list) : INS(push) {
            fullIns = "\tpush {r" + to_string(*(list.begin()));
            for (auto i = list.begin(); i != list.end(); i++) {
                fullIns = fullIns + ",r" + to_string(*i);
            }
            fullIns = fullIns + "}\n";
        }*/

        PUSH(string list) : INS(push) {
            fullIns = "\tpush {" + list + "}\n";
        }
    };

    class POP : public INS {
    public:
        /*POP(initializer_list<int> list) : INS(pop) {
            fullIns = "\tpop {r" + to_string(*(list.begin()));
            for (auto i = list.begin(); i != list.end(); i++) {
                fullIns = fullIns + ",r" + to_string(*i);
            }
            fullIns = fullIns + "}";
        }*/

        POP(string list) : INS(pop) {
            fullIns = "\tpop {" + list + "}\n";
>>>>>>> optimize
        }
    };
}
#endif //COMPILER_BACK_H
