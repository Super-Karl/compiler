//
// Created by duhao on 2021/7/19.
//

#include "back.h"
#include <assert.h>

namespace compiler::back{
    void printInstruction(Instruction instr)
    {
        switch(instr){
            case Instruction::NOP:
                std::cout<<"";
                break;
            case Instruction::ADC:
                std::cout<<"ADC";
                break;
            case Instruction::ADD:
                std::cout<<"ADD";
                break;
            case Instruction::AND:
                std::cout<<"AND";
                break;
            case Instruction::B:
                std::cout<<"B";
                break;
            case Instruction::BIC:
                std::cout<<"BIC";
                break;
            case Instruction::BL:
                std::cout<<"BL";
                break;
            case Instruction::BLX:
                std::cout<<"BLX";
                break;
            case Instruction::BX:
                std::cout<<"BX";
                break;
            case Instruction::CDP:
                std::cout<<"CDP";
                break;
            case Instruction::CMN:
                std::cout<<"CMN";
                break;
            case Instruction::CMP:
                std::cout<<"CMP";
                break;
            case Instruction::EOR:
                std::cout<<"EOR";
                break;
            case Instruction::LDR:
                std::cout<<"LDR";
                break;
            case Instruction::MCR:
                std::cout<<"MCR";
                break;
            case Instruction::MOV:
                std::cout<<"MOV";
                break;
            case Instruction::MRC:
                std::cout<<"MRC";
                break;
            case Instruction::MRS:
                std::cout<<"MRS";
                break;
            case Instruction::MSR:
                std::cout<<"MSR";
                break;
            case Instruction::MUL:
                std::cout<<"MUL";
                break;
            case Instruction::MLA:
                std::cout<<"MLA";
                break;
            case Instruction::MVN:
                std::cout<<"MVN";
                break;
            case Instruction::ORR:
                std::cout<<"ORR";
                break;
            case Instruction::RSB:
                std::cout<<"RSB";
                break;
            case Instruction::RSC:
                std::cout<<"RSC";
                break;
            case Instruction::SBC:
                std::cout<<"SBC";
                break;
            case Instruction::STC:
                std::cout<<"STC";
                break;
            case Instruction::STM:
                std::cout<<"STM";
                break;
            case Instruction::STR:
                std::cout<<"STR";
                break;
            case Instruction::SUB:
                std::cout<<"SUB";
                break;
            case Instruction::SWI:
                std::cout<<"SWI";
                break;
            case Instruction::SWP:
                std::cout<<"SWP";
                break;
            case Instruction::SDIV:
                std::cout<<"SDIV";
                break;
            case Instruction::TEQ:
                std::cout<<"TEQ";
                break;
        }
    }
    void printBarcode(BarCode barCode)
    {
        switch (barCode) {
            case BarCode::NOP:
                std::cout<<"";
                break;
            case BarCode::EQ:
                std::cout<<"EQ";
                break;
            case BarCode::NE:
                std::cout<<"NE";
                break;
            case BarCode::CS:
                std::cout<<"CS";
                break;
            case BarCode::CC:
                std::cout<<"CC";
                break;
            case BarCode::MI:
                std::cout<<"MI";
                break;
            case BarCode::PL:
                std::cout<<"PL";
                break;
            case BarCode::VS:
                std::cout<<"VS";
                break;
            case BarCode::VC:
                std::cout<<"VC";
                break;
            case BarCode::HI:
                std::cout<<"HI";
                break;
            case BarCode::LS:
                std::cout<<"GE";
                break;
            case BarCode::LT:
                std::cout<<"LT";
                break;
            case BarCode::GT:
                std::cout<<"GT";
                break;
            case BarCode::LE:
                std::cout<<"LE";
                break;
            case BarCode::AL:
                std::cout<<"AL ";
                break;
        }
    }
    void printSuffix(Suffix suffix){
        switch (suffix) {
            case Suffix::SNOP:
                std::cout<<"";
                break;
            case Suffix::S:
                std::cout<<"S";
                break;
            case Suffix::e_mark:
                std::cout<<"!";
                break;
        }
    }
    void printEQUKeywords(EQUKeywords equKeywords){
        switch (equKeywords) {
            case EQUKeywords::nop:
                std::cout<<"";
                break;
            case EQUKeywords::byte:
                std::cout<<".byte";
                break;
            case EQUKeywords::hword:
                std::cout<<".hword";
                break;
            case EQUKeywords::Eshort:
                std::cout<<".eshort";
                break;
            case EQUKeywords::word:
                std::cout<<".word";
                break;
            case EQUKeywords::Eint:
                std::cout<<".eint";
                break;
            case EQUKeywords::quad:
                std::cout<<".quad";
                break;
            case EQUKeywords::Efloat:
                std::cout<<".efloat";
                break;
            case EQUKeywords::Edouble:
                std::cout<<".edouble";
                break;
            case EQUKeywords::ascii:
                std::cout<<".ascii";
                break;
            case EQUKeywords::asciz:
                std::cout<<".asciz";
                break;
            case EQUKeywords::zero:
                std::cout<<".zero";
                break;
            case EQUKeywords::space:
                std::cout<<".space";
                break;
            case EQUKeywords::code32:
                std::cout<<".code32";
                break;
            case EQUKeywords::code16:
                std::cout<<".code16";
                break;
            case EQUKeywords::fpu:
                std::cout<<".fpu";
                break;
            case EQUKeywords::section:
                std::cout<<".section";
                break;
            case EQUKeywords::text:
                std::cout<<".text";
                break;
            case EQUKeywords::data:
                std::cout<<".data";
                break;
            case EQUKeywords::bss:
                std::cout<<".bss";
                break;
            case EQUKeywords::align:
                std::cout<<".align";
                break;
            case EQUKeywords::type:
                std::cout<<".type";
                break;
            case EQUKeywords::size:
                std::cout<<".size";
                break;
            case EQUKeywords::org:
                std::cout<<".org";
                break;
            case EQUKeywords::global:
                std::cout<<".global";
                break;
            case EQUKeywords::include:
                std::cout<<".include";
                break;
            case EQUKeywords::end:
                std::cout<<".end";
                break;
            case EQUKeywords::_start:
                std::cout<<".start";
                break;
            case EQUKeywords::macro:
                std::cout<<".macro";
                break;
            case EQUKeywords::endm:
                std::cout<<".endm";
                break;
            case EQUKeywords::exitm:
                std::cout<<".exitm";
                break;
            case EQUKeywords::ifdef:
                std::cout<<".ifdef";
                break;
            case EQUKeywords::Eelse:
                std::cout<<".eelse";
                break;
            case EQUKeywords::endif:
                std::cout<<".endif";
                break;
            case EQUKeywords::rept:
                std::cout<<".rept";
                break;
            case EQUKeywords::equ:
                std::cout<<".equ";
                break;
            case EQUKeywords::list:
                std::cout<<".list";
                break;
            case EQUKeywords::nolist:
                std::cout<<".nolist";
                break;
            case EQUKeywords::req:
                std::cout<<".req";
                break;
            case EQUKeywords::unreq:
                std::cout<<".unreq";
                break;
            case EQUKeywords::pool:
                std::cout<<".pool";
                break;
        }
    }
    void printType(TYPE type){
        switch (type) {
            case TYPE::NOP:
                std::cout<<"";
                break;
            case TYPE::object:
                std::cout<<"%object";
                break;
            case TYPE::function:
                std::cout<<"%function";
                break;
        }
    }
    void printstmType(stmType stmtype){
        switch (stmtype) {
            case stmType::NOP:
                std::cout<<"";
                break;
            case stmType::IA:
                std::cout<<"IA";
                break;
            case stmType::IB:
                std::cout<<"IB";
                break;
            case stmType::DA:
                std::cout<<"DA";
                break;
            case stmType::DB:
                std::cout<<"DB";
                break;
            case stmType::FD:
                std::cout<<"FD";
                break;
            case stmType::ED:
                std::cout<<"ED";
                break;
            case stmType::FA:
                std::cout<<"FA";
                break;
            case stmType::EA:
                std::cout<<"EA";
                break;
        }
    }
}
