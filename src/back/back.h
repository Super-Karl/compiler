//
// Created by duhao on 2021/7/14.
//目前只涉及ARM的部分,没有涉及Thumb指令
//在本段代码中,程序只是进行 了转换,并没有理解 做了什么,所以并不需要做包括检查CPSR等操作.
//所以目前感觉没有必要设置CPSR的类,因为并没有在指令中显性说明,所以只是翻译句子的话没有必要写CPSR
//

#ifndef COMPILER_BACK_H
#define COMPILER_BACK_H
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
        }
    };
}
#endif //COMPILER_BACK_H
