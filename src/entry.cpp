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
#include <fstream>

using namespace std;

using Hash = std::unordered_map<string, int>;
int main(int argc, char **argv) {
    string inputfile=argv[4];
    FILE *input =fopen(argv[4],"r");
    if(input==nullptr){
        cout<<"输出文件错误";
    }
    ofstream outfile;
    string outputfile=argv[3];
    outfile.open(outputfile);
  //auto *argParser = new compiler::controller::ArgParser(argc, argv);

  auto *root = compiler::controller::generator::generate(input);

//  root->print();
  Hash constTbale;
  compiler::astpassir::FirstPassRoot(root, constTbale);

  auto ir = compiler::controller::generator::genIR(root);
  //if (argParser->printIR)freopen("gen.ir","w",stdout);
  compiler::controller::generator::printIR(ir);
  auto arm = compiler::back::genarm::genBack(ir);
  std::fstream file( "testcase.s", std::ios::out );
  for(auto var:arm){
      outfile<<var->print();
      outfile.close();
      //std::cout<<var->print();
  }
  return 0;
}