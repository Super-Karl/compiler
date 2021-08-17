//
// Created by wnx on 2021/8/16.
//

#include "deadCodeElimination.h"

using namespace compiler::front::ast;

int Table::id = 0;
unordered_map<int,Table*> Table::tableMap;
bool Table::inRecord(std::string &s) {
  try {
    return usedMap.at(s);
  }catch (...){
    if (this->father != nullptr){
      return father->inRecord(s);
    }else{
      return false;
    }
  }
}
void Table::addUse(std::string & s) {
  try {
    usedMap.at(s);
    usedMap[s] = true;
  }catch (...){
    if (this->father != nullptr){
      father->addUse(s);
    }
  }
}

void Table::addVar(std::string &s) {
  this->usedMap[s] = false;
}

Table* Table::getTableByNodeId(int id) {
  return tableMap.at(id);
}

Table* Table::createTableByNodeId(int id,Table * t) {
  Table *table = new Table(t);
  tableMap.insert({id,table});
  return table;
}

void Table::aboutToDel(list<compiler::front::ast::Expression*>::iterator it){
  needToDel.push_back(it);
}
void Table::setVarLatestUse(compiler::front::ast::Node *node) {}

int Table::getTimeStamp(compiler::front::ast::Node *node) { return 0; }

void Table::setTimeStamp(compiler::front::ast::Node *node) {}

void Table::setVar(const compiler::front::ast::Identifier *identifier) {}

void scanAST(compiler::front::ast::Node *node,Table *table){
  if (node == NULL)return;
  int type = node->nodetype;
  switch (type) {
  case BlockType: {
    Table *newTable = Table::createTableByNodeId(node->uid, table);
    auto blockNode = dynamic_cast<Block *>(node);
    for (auto &item : blockNode->blockItem) {
      scanAST(item, newTable);
    }
  }
  break;
  case AssignStmtType: {
    auto assignStmtNode = dynamic_cast<AssignStmt *>(node);
    scanAST(assignStmtNode->rightExpr, table);
  }
  break;
  case BinaryExpressionType: {
    auto biExpNode = dynamic_cast<BinaryExpression *>(node);
    scanAST(biExpNode->leftExpr, table);
    scanAST(biExpNode->rightExpr, table);
  }
  break;
  case UnaryExpressionType: {
    auto uExp = dynamic_cast<UnaryExpression *>(node);
    scanAST(uExp->right, table);
  }
  break;
  case IfStatementType: {
    auto ifStmt = dynamic_cast<IfStatement *>(node);
    scanAST(ifStmt->cond, table);
    scanAST(ifStmt->trueBlock, table);
    scanAST(ifStmt->elseBlock, table);
  }
  break;
  case WhileStatementType: {
    auto whileStmt = dynamic_cast<WhileStatement *>(node);
    scanAST(whileStmt->cond, table);
    scanAST(whileStmt->loopBlock, table);
  }
  break;
  case FunctionDefineType: {
    auto funcDef = dynamic_cast<FunctionDefine *>(node);
    scanAST(funcDef->body, table);
  }
  break;
  case FunctionCallType: {
    auto funCall = dynamic_cast<FunctionCall *>(node);
    scanAST(funCall->args, table);
  }
  break;
  case FunctionCallArgListType: {
    auto funCallArgs = dynamic_cast<FunctionCallArgList *>(node);
    for (auto &item : funCallArgs->args) {
      scanAST(item, table);
    }
  }
  break;
  case IdentifierType: {
    auto ident = dynamic_cast<Identifier *>(node);
    table->addUse(ident->name);
  }
  break;
  case ReturnStatementType: {
    auto returnStmt = dynamic_cast<ReturnStatement *>(node);
    scanAST(returnStmt->returnExp, table);
  }
  break;
  case ArrayDeclareType: {
    auto arrDecl = dynamic_cast<ArrayDeclare *>(node);
    scanAST(arrDecl->name, table);
  }
  break;
  case ArrayIdentifierType: {
    auto arrIdent = dynamic_cast<ArrayIdentifier *>(node);
    for (auto &item : arrIdent->index) {
      scanAST(item, table);
    }
  }
  break;
  case ArrayInitValType: {
    auto arrInitVal = dynamic_cast<ArrayInitVal *>(node);
    for (auto &item : arrInitVal->initValList) {
      scanAST(item, table);
    }
  }
  break;
  case ArrayDeclareWithInitType: {
    auto arrDeclWithInit = dynamic_cast<ArrayDeclareWithInit *>(node);
    scanAST(arrDeclWithInit->name, table);
    scanAST(arrDeclWithInit->initVal, table);
  }
  break;
  case ConstArrayType: {
    auto constArr = dynamic_cast<ConstArray *>(node);
    scanAST(constArr->initVal, table);
    scanAST(constArr->name, table);
  }
  break;
  case ConstDeclareType: {
    auto constDecl = dynamic_cast<ConstDeclare *>(node);
    scanAST(constDecl->value, table);
    table->addVar(constDecl->name->name);
  }
  break;
  case DeclareStatementType: {
    auto DeclStmt = dynamic_cast<DeclareStatement *>(node);
    for (auto &item : DeclStmt->declareList) {
      scanAST(item, table);
    }
  }
  break;
  case VarDeclareWithInitType: {
    auto varDeclWithInit = dynamic_cast<VarDeclareWithInit *>(node);
    scanAST(varDeclWithInit->value, table);
    table->addVar(varDeclWithInit->name->name);
  }
  break;
  case VarDeclareType: {
    auto varDecl = dynamic_cast<VarDeclare *>(node);
    table->addVar(varDecl->name->name);
  }
  break;
  default:
    return;
  }
}
bool scanTwice(compiler::front::ast::Node *node,Table *table){
  int type = node->nodetype;
  switch (type) {
  case BlockType: {
    Table *getTable = Table::getTableByNodeId(node->uid);
    list<list<Expression*>::iterator> aboutToDel;
    auto blockNode = dynamic_cast<Block *>(node);
    for (auto it = blockNode->blockItem.begin();
         it != blockNode->blockItem.end(); ++it) {
      if (scanTwice(*it, getTable))
        aboutToDel.push_back(it);
    }
    for (auto item : aboutToDel) {
      blockNode->blockItem.erase(item);
    }
    return false;
  } break;
  case AssignStmtType: {
    auto assignStmtNode = dynamic_cast<AssignStmt *>(node);
    auto arrayIdentInAssign = dynamic_cast<ArrayIdentifier*>(assignStmtNode->name);
    if (arrayIdentInAssign){
      return false;
    }
    if (!table->inRecord(assignStmtNode->name->name)) {
      return true;
    }
  } break;
  case IfStatementType: {
    auto ifStmt = dynamic_cast<IfStatement *>(node);
    scanTwice(ifStmt->trueBlock, table);
    scanTwice(ifStmt->elseBlock, table);
    return false;
  } break;
  case WhileStatementType: {
    auto whileStmt = dynamic_cast<WhileStatement *>(node);
    scanTwice(whileStmt->loopBlock, table);
    return false;
  } break;
  case FunctionDefineType: {
    auto funcDef = dynamic_cast<FunctionDefine *>(node);
    scanTwice(funcDef->body, table);
    return false;
  } break;
  case DeclareStatementType: {
    auto declStmt = dynamic_cast<DeclareStatement *>(node);
    list<list<Declare*>::iterator> aboutToDel;
    for (auto it = declStmt->declareList.begin();
    it != declStmt->declareList.end(); ++it) {
      if (scanTwice(*it, table))
        aboutToDel.push_back(it);
    }
    for (auto item : aboutToDel) {
      declStmt->declareList.erase(item);
    }
    if (declStmt->declareList.empty()){
      return true;
    }
  } break;
  case VarDeclareWithInitType: {
    auto varDeclWithInit = dynamic_cast<VarDeclareWithInit *>(node);
    if (!table->inRecord(varDeclWithInit->name->name)) {
      return true;
    }
  } break;
  case VarDeclareType: {
    auto varDecl = dynamic_cast<VarDeclare *>(node);
    if (!table->inRecord(varDecl->name->name)) {
      return true;
    }
  } break;
  case ConstDeclareType: {
    auto constDecl = dynamic_cast<ConstDeclare *>(node);
    if (!table->inRecord(constDecl->name->name))
      return true;
  } break;
  default:
    return false;
  }
  return false;
}
void DeadCodeElimination(compiler::front::ast::AST *root){
  Table *table = new Table();
  for (auto &item:root->codeBlock){
    scanAST(item,table);
  }
  list<list<Node*>::iterator> aboutToDel;
  for (auto it=root->codeBlock.begin();it != root->codeBlock.end(); ++it) {
    if (scanTwice(*it,table))
      aboutToDel.push_back(it);
  }
  for(auto &item:aboutToDel){
    root->codeBlock.erase(item);
  }
}
