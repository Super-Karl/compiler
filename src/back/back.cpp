//
// Created by duhao on 2021/7/19.
//

#include "back.h"
#include<fstream>
#include <assert.h>

namespace compiler::back{
    void printInstruction(Instruction instr)
    {
        std::fstream outfile;
        outfile.open("result.s", std::ios::app);
        switch(instr){
            case Instruction::NOP:
                outfile<<"";
                std::cout<<"";
                break;
            case Instruction::ADC:
                outfile<<"ADC";
                std::cout<<"ADC";
                break;
            case Instruction::ADD:
                outfile<<"ADD";
                std::cout<<"ADD";
                break;
            case Instruction::AND:
                outfile<<"AND";
                std::cout<<"AND";
                break;
            case Instruction::B:
                outfile<<"B";
                std::cout<<"B";
                break;
            case Instruction::BIC:
                outfile<<"BIC";
                std::cout<<"BIC";
                break;
            case Instruction::BL:
                outfile<<"BL";
                std::cout<<"BL";
                break;
            case Instruction::BLX:
                outfile<<"BLX";
                std::cout<<"BLX";
                break;
            case Instruction::BX:
                outfile<<"BX";
                std::cout<<"BX";
                break;
            case Instruction::CDP:
                outfile<<"CDP";
                std::cout<<"CDP";
                break;
            case Instruction::CMN:
                outfile<<"CMN";
                std::cout<<"CMN";
                break;
            case Instruction::CMP:
                outfile<<"CMP";
                std::cout<<"CMP";
                break;
            case Instruction::EOR:
                outfile<<"EOR";
                std::cout<<"EOR";
                break;
            case Instruction::LDR:
                outfile<<"LDR";
                std::cout<<"LDR";
                break;
            case Instruction::MCR:
                outfile<<"MCR";
                std::cout<<"MCR";
                break;
            case Instruction::MOV:
                outfile<<"MOV";
                std::cout<<"MOV";
                break;
            case Instruction::MRC:
                outfile<<"MRC";
                std::cout<<"MRC";
                break;
            case Instruction::MRS:
                outfile<<"MRS";
                std::cout<<"MRS";
                break;
            case Instruction::MSR:
                outfile<<"MSR";
                std::cout<<"MSR";
                break;
            case Instruction::MUL:
                outfile<<"MUL";
                std::cout<<"MUL";
                break;
            case Instruction::MLA:
                outfile<<"MLA";
                std::cout<<"MLA";
                break;
            case Instruction::MVN:
                outfile<<"MVN";
                std::cout<<"MVN";
                break;
            case Instruction::ORR:
                outfile<<"ORR";
                std::cout<<"ORR";
                break;
            case Instruction::RSB:
                outfile<<"RSB";
                std::cout<<"RSB";
                break;
            case Instruction::RSC:
                outfile<<"RSC";
                std::cout<<"RSC";
                break;
            case Instruction::SBC:
                outfile<<"SBC";
                std::cout<<"SBC";
                break;
            case Instruction::STC:
                outfile<<"STC";
                std::cout<<"STC";
                break;
            case Instruction::STM:
                outfile<<"STM";
                std::cout<<"STM";
                break;
            case Instruction::STR:
                outfile<<"STR";
                std::cout<<"STR";
                break;
            case Instruction::SUB:
                outfile<<"SUB";
                std::cout<<"SUB";
                break;
            case Instruction::SWI:
                outfile<<"SWI";
                std::cout<<"SWI";
                break;
            case Instruction::SWP:
                outfile<<"SWP";
                std::cout<<"SWP";
                break;
            case Instruction::SDIV:
                outfile<<"SDIV";
                std::cout<<"SDIV";
                break;
            case Instruction::TEQ:
                outfile<<"TEQ";
                std::cout<<"TEQ";
                break;
        }
        outfile.close();
    }
    void printBarcode(BarCode barCode)
    {
        std::fstream outfile;
        outfile.open("result.s",std::ios::app);
        switch (barCode) {
            case BarCode::NOP:
                outfile<<"";
                std::cout<<"";
                break;
            case BarCode::EQ:
                outfile<<"EQ";
                std::cout<<"EQ";
                break;
            case BarCode::NE:
                outfile<<"NE";
                std::cout<<"NE";
                break;
            case BarCode::CS:
                outfile<<"CS";
                std::cout<<"CS";
                break;
            case BarCode::CC:
                outfile<<"CC";
                std::cout<<"CC";
                break;
            case BarCode::MI:
                outfile<<"MI";
                std::cout<<"MI";
                break;
            case BarCode::PL:
                outfile<<"PL";
                std::cout<<"PL";
                break;
            case BarCode::VS:
                outfile<<"VS";
                std::cout<<"VS";
                break;
            case BarCode::VC:
                outfile<<"VC";
                std::cout<<"VC";
                break;
            case BarCode::HI:
                outfile<<"HI";
                std::cout<<"HI";
                break;
            case BarCode::LS:
                outfile<<"LS";
                std::cout<<"LS";
                break;
            case BarCode::LT:
                outfile<<"LT";
                std::cout<<"LT";
                break;
            case BarCode::GT:
                outfile<<"GT";
                std::cout<<"GT";
                break;
            case BarCode::LE:
                outfile<<"LE";
                std::cout<<"LE";
                break;
            case BarCode::AL:
                outfile<<"AL";
                std::cout<<"AL";
                break;
        }
        outfile.close();
    }
    void printSuffix(Suffix suffix){
        std::fstream outfile;
        outfile.open("result.s",std::ios::app);

        switch (suffix) {
            case Suffix::SNOP:
                outfile<<"";
                std::cout<<"";
                break;
            case Suffix::S:
                outfile<<"S";
                std::cout<<"S";
                break;
            case Suffix::e_mark:
                outfile<<"!";
                std::cout<<"!";
                break;
        }
        outfile.close();
    }
    void printEQUKeywords(EQUKeywords equKeywords){
        std::fstream outfile;
        outfile.open("result.s",std::ios::app);
        switch (equKeywords) {
            case EQUKeywords::nop:
                outfile<<"";
                std::cout<<"";
                break;
            case EQUKeywords::byte:
                outfile<<".byte";
                std::cout<<".byte";
                break;
            case EQUKeywords::hword:
                outfile<<"hword";
                std::cout<<".hword";
                break;
            case EQUKeywords::Eshort:
                outfile<<".eshort";
                std::cout<<".eshort";
                break;
            case EQUKeywords::word:
                outfile<<".word";
                std::cout<<".word";
                break;
            case EQUKeywords::Eint:
                outfile<<".Eint";
                std::cout<<".eint";
                break;
            case EQUKeywords::quad:
                outfile<<".quad";
                std::cout<<".quad";
                break;
            case EQUKeywords::Efloat:
                outfile<<".Efloat";
                std::cout<<".efloat";
                break;
            case EQUKeywords::Edouble:
                outfile<<".Edouble";
                std::cout<<".edouble";
                break;
            case EQUKeywords::ascii:
                outfile<<".ascii";
                std::cout<<".ascii";
                break;
            case EQUKeywords::asciz:
                outfile<<".asciz";
                std::cout<<".asciz";
                break;
            case EQUKeywords::zero:
                outfile<<".zero";
                std::cout<<".zero";
                break;
            case EQUKeywords::space:
                outfile<<".sapce";
                std::cout<<".space";
                break;
            case EQUKeywords::code32:
                outfile<<".code32";
                std::cout<<".code32";
                break;
            case EQUKeywords::code16:
                outfile<<".code16";
                std::cout<<".code16";
                break;
            case EQUKeywords::fpu:
                outfile<<".fpu";
                std::cout<<".fpu";
                break;
            case EQUKeywords::section:
                outfile<<".section";
                std::cout<<".section";
                break;
            case EQUKeywords::text:
                outfile<<".text";
                std::cout<<".text";
                break;
            case EQUKeywords::data:
                outfile<<".data";
                std::cout<<".data";
                break;
            case EQUKeywords::bss:
                outfile<<".bss";
                std::cout<<".bss";
                break;
            case EQUKeywords::align:
                outfile<<".align";
                std::cout<<".align";
                break;
            case EQUKeywords::type:
                outfile<<".type";
                std::cout<<".type";
                break;
            case EQUKeywords::size:
                outfile<<".size";
                std::cout<<".size";
                break;
            case EQUKeywords::org:
                outfile<<".org";
                std::cout<<".org";
                break;
            case EQUKeywords::global:
                outfile<<".global";
                std::cout<<".global";
                break;
            case EQUKeywords::include:
                outfile<<".include";
                std::cout<<".include";
                break;
            case EQUKeywords::end:
                outfile<<".end";
                std::cout<<".end";
                break;
            case EQUKeywords::_start:
                outfile<<"_start";
                std::cout<<"_start";
                break;
            case EQUKeywords::macro:
                outfile<<".macro";
                std::cout<<".macro";
                break;
            case EQUKeywords::endm:
                outfile<<".endm";
                std::cout<<".endm";
                break;
            case EQUKeywords::exitm:
                outfile<<".exitm";
                std::cout<<".exitm";
                break;
            case EQUKeywords::ifdef:
                outfile<<".ifdef";
                std::cout<<".ifdef";
                break;
            case EQUKeywords::Eelse:
                outfile<<".Eelse";
                std::cout<<".eelse";
                break;
            case EQUKeywords::endif:
                outfile<<".endif";
                std::cout<<".endif";
                break;
            case EQUKeywords::rept:
                outfile<<".rept";
                std::cout<<".rept";
                break;
            case EQUKeywords::equ:
                outfile<<".equ";
                std::cout<<".equ";
                break;
            case EQUKeywords::list:
                outfile<<".list";
                std::cout<<".list";
                break;
            case EQUKeywords::nolist:
                outfile<<".nolist";
                std::cout<<".nolist";
                break;
            case EQUKeywords::req:
                outfile<<".req";
                std::cout<<".req";
                break;
            case EQUKeywords::unreq:
                outfile<<".unreq";
                std::cout<<".unreq";
                break;
            case EQUKeywords::pool:
                outfile<<".pool";
                std::cout<<".pool";
                break;
        }
        outfile.close();
    }
    void printType(TYPE type){
        std::fstream outfile;
        outfile.open("result.s",std::ios::app);
        switch (type) {
            case TYPE::NOP:
                outfile<<"";
                std::cout<<"";
                break;
            case TYPE::object:
                outfile<<"%object";
                std::cout<<"%object";
                break;
            case TYPE::function:
                outfile<<"%function";
                std::cout<<"%function";
                break;
        }
        outfile.close();
    }
    void printstmType(stmType stmtype){
        std::fstream outfile;
        outfile.open("result.s",std::ios::app);
        switch (stmtype) {
            case stmType::NOP:
                outfile<<"";
                std::cout<<"";
                break;
            case stmType::IA:
                outfile<<"IA";
                std::cout<<"IA";
                break;
            case stmType::IB:
                outfile<<"IB";
                std::cout<<"IB";
                break;
            case stmType::DA:
                outfile<<"DA";
                std::cout<<"DA";
                break;
            case stmType::DB:
                outfile<<"DB";
                std::cout<<"DB";
                break;
            case stmType::FD:
                outfile<<"FD";
                std::cout<<"FD";
                break;
            case stmType::ED:
                outfile<<"ED";
                std::cout<<"ED";
                break;
            case stmType::FA:
                outfile<<"FA";
                std::cout<<"FA";
                break;
            case stmType::EA:
                outfile<<"EA";
                std::cout<<"EA";
                break;
        }
        outfile.close();
    }
}
