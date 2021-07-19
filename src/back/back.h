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
        TEQ,
        TST,
        NOP,
    };//TODO 加上除法(没有除法?)
    enum class BarCode{
        //生成的条件码
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
        BNOP
    };
    enum class Suffix{
        //指令的后缀,只有!和 S两种
        S,
        e_mark,//指!
        SNOP
    };
    //加点或符号会标错,所以用其他符号先代替
    enum class EQUKeywords{
        //定义伪操作
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
       pool,
       nop
    };
    //前面应该加上%
    enum class TYPE{
        object,
        function
    };

    //现在是标号域
    class LABEL{
    public:
        std::string LabelName;
        LABEL(std::string name):LabelName(name){};
        LABEL(){};
    };

    //操作助记符域
    class OPERATION{
        //操作助记符
    public:
        Instruction instruction;
        BarCode barcode;
        OPERATION(Instruction instr,BarCode bcode):instruction(instr),barcode(bcode){};
        OPERATION(Instruction instr):instruction(instr){};
        OPERATION(){};
    };

    //操作数域
    //寄存器后面可能会跟一个如LSL#1,代表左移三位,所以需要定义一个类来作为这个操作
    class OffsetOperate{
        public:
            Instruction OffsetInstr;
            int OffsetNum;
            OffsetOperate(Instruction instr,int num):OffsetInstr(instr),OffsetNum(num){};
    };
    //操作数类
    class OperateNum{
    public:
     OperateNum() = default;
     virtual OperateNum *getThis(){
         return this;
     }
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
        Direct_Reg(std::string name, Suffix suf);
        Direct_Reg(std::string name);
        Direct_Reg *getThis() override {
            return this;
        }
    };
    class Indirect_Reg:public OperateNum{
    public:
        std::string RegName;
        int RegNum;
        int Offset;
        //TODO 完成这两个类的构造,并不确定是否需要这个
        Indirect_Reg(std::string name,int Offset=0);
        Indirect_Reg(int num,int Offset=0);
    };

    class ImmNum:public OperateNum{
    public:
        int value;
        ImmNum(int num):value(num){};
        ImmNum *getThis() override{
            return this;
        }
    };

    class ConstNum:public OperateNum{
    public:
        int value;
        ConstNum(int num):value(num){};
        ConstNum *getThis() override{
            return this;
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
    };
//三大部分构造完毕,注释域不用管
//将三大部分整合成一个语句
    class Sentence{
    public:
        Sentence() = default;
        virtual Sentence  *getThis(){
            return this;
        }
    };
    //正常指令的类型
    class Instr_Sentence:public Sentence{
    public:
        LABEL label;
        OPERATION operation;
        OPERAND operand;
        LABEL b_label;//跳转的话是直接LABEL
        //Instr_Sentence()=default;
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
    };
    //伪指令类型(因为伪指令的形式比较多,想写在一起,也有合并的可能,但是目前先写开)
    //代码块类型声明
    class BlockDeclaration:public  Sentence{
    public:
        BarCode barcode;
        BlockDeclaration(BarCode bcode):barcode(bcode){};
        BlockDeclaration *getThis() override{
            return this;
        }
    };
    //变量类型声明(全局/局部)
    class VarDeclaration:public Sentence{
    public:
        BarCode barcode;
        LABEL label;
        VarDeclaration(BarCode bcode,LABEL label1):barcode(bcode), label(label1){};
        VarDeclaration *getThis() override{
            return this;
        }
    };
    class TypeDeclaration:public  Sentence{
    public:
        BarCode barcode;//???
        LABEL label;
        TYPE type;
        TypeDeclaration(BarCode bcode,LABEL label1,TYPE type1):barcode(bcode), label(label1),type(type1){};
        TypeDeclaration *getThis() override{
            return this;
        }
    };
}
#endif //COMPILER_BACK_H
