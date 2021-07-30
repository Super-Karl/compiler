//
// Created by wnx on 2021/6/11.
//
#include <unordered_map>
#include "front/ast/AstNode.h"
#include "controller/controller.h"
#include "astpass/astpass.h"
#include "back/back.h"
#include "back/generateBack.h"

using namespace std;

using Hash = std::unordered_map<string, int>;
int main(int argc, char **argv) {
    //auto *argParser = new compiler::controller::ArgParser(argc, argv);
    string inputfile = argv[3];

    FILE* input = fopen(inputfile.substr(1,inputfile.size()-1).c_str(),"r");
    if(input==NULL)
    {
        cout<<"读取文件错误";
        return 0;
    }
    auto *root = compiler::controller::generator::generate(input);
    root->print();
    Hash constTbale;
    compiler::astpassir::FirstPassRoot(root,constTbale);
    root->print();

    //生成后端
    list<compiler::back::INS*> backlist = compiler::back::generateBack(root);
    compiler::back::printASM(backlist);

    ofstream outfile;
    string outputfile = argv[4];
    outfile.open(outputfile);
    for(auto i:backlist)
    {
        outfile<<i->getFullIns();
    }
    return 0;
}