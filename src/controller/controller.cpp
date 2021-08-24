//
// Created by wnx on 2021/6/13.
//

#include "controller.h"
#include <iostream>

extern int yydebug;

namespace compiler::controller {
  ArgParser::ArgParser(int argc, char **argv) {
    yydebug = 0;
    bool writeToFile = false;
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        if (std::string(argv[i]) == "-o") {
          writeToFile = true;
        }
        if (std::string(argv[i]) == "-Optimize") {
          this->isOptimize = true;
        }
        if (std::string(argv[i]) == "-printAST") {
          this->printAST = true;
        }
        if (std::string(argv[i]) == "-printIR") {
          this->printIR = true;
        }
        if (std::string(argv[i]) == "-debug") {
          yydebug = 1;
        }
        if (std::string(argv[i]) == "-S") {
        }
      }
      else {
        if (writeToFile) {
          if (std::string(argv[i]) == "-") {
            output = &std::cout;
          } else
            output = new std::ofstream(argv[i], std::ofstream::out);
        } else {
          if (std::string(argv[i]) == "-")
            input = stdin;
          else
            input = fopen(argv[i], "r");
        }
        writeToFile = true;
      }
    }
  }
}// namespace compiler::controller

extern int yyparse();

extern int yylex_destroy();

extern void yyset_lineno(int _line_number);

extern void yyset_in(FILE *_in_str);

namespace compiler::controller::generator {
  compiler::front::ast::AST *root = nullptr;

  compiler::front::ast::AST *generate(FILE *input) {
    yyset_in(input);
    yyset_lineno(1);
    yyparse();
    yylex_destroy();
    return root;
  }

  compiler::mid::ir::IRList genIR(compiler::front::ast::AST *root) {
    compiler::mid::ir::IRList ir;
    auto *record = new compiler::mid::ir::RecordTable();
    root->genIR(ir, record);
    return ir;
  }

  void printIR(compiler::mid::ir::IRList ir) {
    //std::cout << "------------------------------\n\nIR:\n\n";

    for (auto i : ir) {
      i->print();
    }
  }
}// namespace compiler::controller::generator
