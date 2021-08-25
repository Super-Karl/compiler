//
// Created by wnx on 2021/8/16.
//

#ifndef COMPILER_DEADCODEELIMINATION_H
#define COMPILER_DEADCODEELIMINATION_H

#include "front/ast/AstNode.h"
#include <unordered_map>

class Table {
public:
  Table *father;
  unordered_map<string,bool> usedMap;
  static unordered_map<int,Table*> tableMap;
  list<list<compiler::front::ast::Expression*>::iterator> needToDel;
  Table() = default;

  Table(Table *t):father(t){}

  bool inRecord(std::string&);

  void addUse(std::string &);

  void addVar(std::string &);

  void aboutToDel(list<compiler::front::ast::Expression*>::iterator it);

  static Table* getTableByNodeId(int id);

  static Table* createTableByNodeId(int id,Table* );

  void setVarLatestUse(compiler::front::ast::Node *node);

  int getTimeStamp(compiler::front::ast::Node *node);

  void setTimeStamp(compiler::front::ast::Node *node);

  void setVar(const compiler::front::ast::Identifier *identifier);

  std::vector<std::vector<compiler::front::ast::Node *>::iterator> deleteList;

  std::unordered_map<compiler::front::ast::Node *, int> timeStamp;

  std::unordered_map<std::string, int> varLatestUse;

private:
  static int id;
};


void scanAST(compiler::front::ast::Node *node,Table *table);
bool scanTwice(compiler::front::ast::Node *node,Table *table);
void DeadCodeElimination(compiler::front::ast::AST *root);
#endif // COMPILER_DEADCODEELIMINATION_H
