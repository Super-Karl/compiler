//
// Created by wnx on 2021/6/11.
//
#include "astpass/astpass.h"
#include "controller/controller.h"
#include "back/genback.h"
#include "front/ast/AstNode.h"
#include "back/back.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

using Hash = std::unordered_map<string, int>;
int main(int argc, char **argv) {
    string inputfile=argv[4];
    FILE *input =fopen(argv[4],"r");
    if(input==nullptr){
        cout<<"输出文件错误";
    }
    auto *root = compiler::controller::generator::generate(input);
    Hash constTbale;
    compiler::astpassir::FirstPassRoot(root, constTbale);

  auto ir = compiler::controller::generator::genIR(root);
  if (argParser->printIR)freopen("gen.ir","w",stdout);
  compiler::controller::generator::printIR(ir);
  return 0;
}
