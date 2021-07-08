//
// Created by wnx on 2021/6/11.
//
#include <iostream>
#include <unordered_map>
#include "front/ast/AstNode.h"
#include "controller/controller.h"
#include "astpass/astpass.h"

using namespace std;

using Hash = std::unordered_map<string, int>;
int main(int argc, char **argv) {
    auto *argParser = new compiler::controller::ArgParser(argc, argv);

    auto *root = compiler::controller::generator::generate(argParser->input);
    if (argParser->printAST)
        root->print();
    Hash constTbale;
    compiler::astpassir::FirstPassRoot(root,constTbale);
    root->print();
    return 0;
}