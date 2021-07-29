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
    FILE* input = fopen(argv[3],"r");
    auto *root = compiler::controller::generator::generate(input);
    root->print();
    Hash constTbale;
    compiler::astpassir::FirstPassRoot(root,constTbale);
    root->print();

    //生成后端
    list<compiler::back::INS*> backlist = compiler::back::generateBack(root);
    compiler::back::printASM(backlist);

    ofstream outfile;
    outfile.open(argv[4]);
    for(auto i:backlist)
    {
        outfile<<i->getFullIns();
    }
    return 0;
}