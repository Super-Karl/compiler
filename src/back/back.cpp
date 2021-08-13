//
// Created by duhao on 2021/7/19.
//

#include "back.h"
#include<fstream>
#include <assert.h>

namespace compiler::back{
    std::string printInstruction(Instruction instr)
    {
        std::string word;
        ////std::fstream ////outfile;
        ////outfile.open("testcase.s", std::ios::app);
        switch(instr){
            case Instruction::NOP:
                ////outfile<<"";
                word="";
                break;
            case Instruction::ADC:
                ////outfile<<"ADC";
                word="ADC";
                break;
            case Instruction::ADD:
                ////outfile<<"ADD";
                word="ADD";
                break;
            case Instruction::AND:
                ////outfile<<"AND";
                word="AND";
                break;
            case Instruction::B:
                ////outfile<<"B";
                word="B";
                break;
            case Instruction::BIC:
                ////outfile<<"BIC";
                word="BIC";
                break;
            case Instruction::BL:
                ////outfile<<"BL";
                word="BL";
                break;
            case Instruction::BLX:
                ////outfile<<"BLX";
                word="BLX";
                break;
            case Instruction::BX:
                ////outfile<<"BX";
                word="BX";
                break;
            case Instruction::CDP:
                ////outfile<<"CDP";
                word="CDP";
                break;
            case Instruction::CMN:
                ////outfile<<"CMN";
                word="CMN";
                break;
            case Instruction::CMP:
                ////outfile<<"CMP";
                word="CMP";
                break;
            case Instruction::EOR:
                ////outfile<<"EOR";
                word="EOR";
                break;
            case Instruction::LDR:
                ////outfile<<"LDR";
                word="LDR";
                break;
            case Instruction::MCR:
                ////outfile<<"MCR";
                word="MCR";
                break;
            case Instruction::MOV:
                ////outfile<<"MOV";
                word="MOV";
                break;
            case Instruction::MRC:
                ////outfile<<"MRC";
                word="MRC";
                break;
            case Instruction::MRS:
                ////outfile<<"MRS";
                word="MRS";
                break;
            case Instruction::MSR:
                ////outfile<<"MSR";
                word="MSR";
                break;
            case Instruction::MUL:
                ////outfile<<"MUL";
                word="MUL";
                break;
            case Instruction::MLA:
                ////outfile<<"MLA";
                word="MLA";
                break;
            case Instruction::MVN:
                ////outfile<<"MVN";
                word="MVN";
                break;
            case Instruction::ORR:
                ////outfile<<"ORR";
                word="ORR";
                break;
            case Instruction::RSB:
                ////outfile<<"RSB";
                word="RSB";
                break;
            case Instruction::RSC:
                ////outfile<<"RSC";
                word="RSC";
                break;
            case Instruction::SBC:
                ////outfile<<"SBC";
                word="SBC";
                break;
            case Instruction::STC:
                ////outfile<<"STC";
                word="STC";
                break;
            case Instruction::STM:
                ////outfile<<"STM";
                word="STM";
                break;
            case Instruction::STR:
                ////outfile<<"STR";
                word="STR";
                break;
            case Instruction::SUB:
                ////outfile<<"SUB";
                word="SUB";
                break;
            case Instruction::SWI:
                ////outfile<<"SWI";
                word="SWI";
                break;
            case Instruction::SWP:
                ////outfile<<"SWP";
                word="SWP";
                break;
            case Instruction::SDIV:
                ////outfile<<"SDIV";
                word="SDIV";
                break;
            case Instruction::TEQ:
                ////outfile<<"TEQ";
                word="TEQ";
                break;
            case Instruction::MOV32:
                ////outfile<<"";
                word="MOV32";
                break;
        }
        ////outfile.close();
        return word;
    }
    std::string printBarcode(BarCode barCode)
    {
        std::string word;
        ////std::fstream ////outfile;
        ////outfile.open("testcase.s",std::ios::app);
        switch (barCode) {
            case BarCode::NOP:
                ////outfile<<"";
                word="";
                break;
            case BarCode::EQ:
                ////outfile<<"EQ";
                word="EQ";
                break;
            case BarCode::NE:
                ////outfile<<"NE";
                word="NE";
                break;
            case BarCode::CS:
                ////outfile<<"CS";
                word="CS";
                break;
            case BarCode::CC:
                ////outfile<<"CC";
                word="CC";
                break;
            case BarCode::MI:
                ////outfile<<"MI";
                word="MI";
                break;
            case BarCode::PL:
                ////outfile<<"PL";
                word="PL";
                break;
            case BarCode::VS:
                ////outfile<<"VS";
                word="VS";
                break;
            case BarCode::VC:
                ////outfile<<"VC";
                word="VC";
                break;
            case BarCode::HI:
                ////outfile<<"HI";
                word="HI";
                break;
            case BarCode::LS:
                ////outfile<<"LS";
                word="LS";
                break;
            case BarCode::LT:
                ////outfile<<"LT";
                word="LT";
                break;
            case BarCode::GT:
                ////outfile<<"GT";
                word="GT";
                break;
            case BarCode::LE:
                ////outfile<<"LE";
                word="LE";
                break;
            case BarCode::AL:
                ////outfile<<"AL";
                word="AL";
                break;
        }
        ////outfile.close();
        return word;
    }
    std::string printSuffix(Suffix suffix){
        std::string word;
        ////std::fstream ////outfile;
        ////outfile.open("testcase.s",std::ios::app);

        switch (suffix) {
            case Suffix::SNOP:
                ////outfile<<"";
                word="";
                break;
            case Suffix::S:
                ////outfile<<"S";
                word="S";
                break;
            case Suffix::e_mark:
                ////outfile<<"!";
                word="!";
                break;
        }
        ////outfile.close();
        return word;
    }
    std::string printEQUKeywords(EQUKeywords equKeywords){
        std::string word;
        ////std::fstream ////outfile;
        ////outfile.open("testcase.s",std::ios::app);
        switch (equKeywords) {
            case EQUKeywords::nop:
                ////outfile<<"";
                word="";
                break;
            case EQUKeywords::byte:
                ////outfile<<".byte";
                word=".byte";
                break;
            case EQUKeywords::hword:
                ////outfile<<"hword";
                word=".hword";
                break;
            case EQUKeywords::Eshort:
                ////outfile<<".eshort";
                word=".eshort";
                break;
            case EQUKeywords::word:
                ////outfile<<".word";
                word=".word";
                break;
            case EQUKeywords::Eint:
                ////outfile<<".Eint";
                word=".eint";
                break;
            case EQUKeywords::quad:
                ////outfile<<".quad";
                word=".quad";
                break;
            case EQUKeywords::Efloat:
                ////outfile<<".Efloat";
                word=".efloat";
                break;
            case EQUKeywords::Edouble:
                ////outfile<<".Edouble";
                word=".edouble";
                break;
            case EQUKeywords::ascii:
                ////outfile<<".ascii";
                word=".ascii";
                break;
            case EQUKeywords::asciz:
                ////outfile<<".asciz";
                word=".asciz";
                break;
            case EQUKeywords::zero:
                ////outfile<<".zero";
                word=".zero";
                break;
            case EQUKeywords::space:
                ////outfile<<".sapce";
                word=".space";
                break;
            case EQUKeywords::code32:
                ////outfile<<".code32";
                word=".code32";
                break;
            case EQUKeywords::code16:
                ////outfile<<".code16";
                word=".code16";
                break;
            case EQUKeywords::fpu:
                ////outfile<<".fpu";
                word=".fpu";
                break;
            case EQUKeywords::section:
                ////outfile<<".section";
                word=".section";
                break;
            case EQUKeywords::text:
                ////outfile<<".text";
                word=".text";
                break;
            case EQUKeywords::data:
                ////outfile<<".data";
                word=".data";
                break;
            case EQUKeywords::bss:
                ////outfile<<".bss";
                word=".bss";
                break;
            case EQUKeywords::align:
                ////outfile<<".align";
                word=".align";
                break;
            case EQUKeywords::type:
                ////outfile<<".type";
                word=".type";
                break;
            case EQUKeywords::size:
                ////outfile<<".size";
                word=".size";
                break;
            case EQUKeywords::org:
                ////outfile<<".org";
                word=".org";
                break;
            case EQUKeywords::global:
                ////outfile<<".global";
                word=".global";
                break;
            case EQUKeywords::include:
                ////outfile<<".include";
                word=".include";
                break;
            case EQUKeywords::end:
                ////outfile<<".end";
                word=".end";
                break;
            case EQUKeywords::_start:
                ////outfile<<"_start";
                word="_start";
                break;
            case EQUKeywords::macro:
                ////outfile<<".macro";
                word=".macro";
                break;
            case EQUKeywords::endm:
                ////outfile<<".endm";
                word=".endm";
                break;
            case EQUKeywords::exitm:
                ////outfile<<".exitm";
                word=".exitm";
                break;
            case EQUKeywords::ifdef:
                ////outfile<<".ifdef";
                word=".ifdef";
                break;
            case EQUKeywords::Eelse:
                ////outfile<<".Eelse";
                word=".eelse";
                break;
            case EQUKeywords::endif:
                ////outfile<<".endif";
                word=".endif";
                break;
            case EQUKeywords::rept:
                ////outfile<<".rept";
                word=".rept";
                break;
            case EQUKeywords::equ:
                ////outfile<<".equ";
                word=".equ";
                break;
            case EQUKeywords::list:
                ////outfile<<".list";
                word=".list";
                break;
            case EQUKeywords::nolist:
                ////outfile<<".nolist";
                word=".nolist";
                break;
            case EQUKeywords::req:
                ////outfile<<".req";
                word=".req";
                break;
            case EQUKeywords::unreq:
                ////outfile<<".unreq";
                word=".unreq";
                break;
            case EQUKeywords::pool:
                ////outfile<<".pool";
                word=".pool";
                break;
        }
        ////outfile.close();
        return word;
    }
    std::string printType(TYPE type){
        std::string word;
        ////std::fstream ////outfile;
        ////outfile.open("testcase.s",std::ios::app);
        switch (type) {
            case TYPE::NOP:
                ////outfile<<"";
                word="";
                break;
            case TYPE::object:
                ////outfile<<"%object";
                word="%object";
                break;
            case TYPE::function:
                ////outfile<<"%function";
                word="%function";
                break;
        }
        ////outfile.close();
        return word;
    }
    std::string printstmType(stmType stmtype){
        std::string word;
        ////std::fstream ////outfile;
        ////outfile.open("testcase.s",std::ios::app);
        switch (stmtype) {
            case stmType::NOP:
                ////outfile<<"";
                word="";
                break;
            case stmType::IA:
                ////outfile<<"IA";
                word="IA";
                break;
            case stmType::IB:
                ////outfile<<"IB";
                word="IB";
                break;
            case stmType::DA:
                ////outfile<<"DA";
                word="DA";
                break;
            case stmType::DB:
                ////outfile<<"DB";
                word="DB";
                break;
            case stmType::FD:
                ////outfile<<"FD";
                word="FD";
                break;
            case stmType::ED:
                ////outfile<<"ED";
                word="ED";
                break;
            case stmType::FA:
                ////outfile<<"FA";
                word="FA";
                break;
            case stmType::EA:
                ////outfile<<"EA";
                word="EA";
                break;
        }
        ////outfile.close();
        return word;
    }
}
