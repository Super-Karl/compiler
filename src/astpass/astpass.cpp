//
// Created by karl on 2021/7/7.
//
#include "astpass.h"
#include <unordered_map>
#include <iostream>


using namespace compiler::front::ast;

namespace compiler::astpassir {
    /*int caluExpersion(Node *exp, Hash constTbale) {
        switch (exp->nodetype) {
            case NumberExpressionType:
                return static_cast<NumberExpression *>(exp)->value;
            case IdentifierType:
                return constTbale.at(static_cast<Identifier *>(exp)->name);
        }
    }*/

    void FirstPassRoot(compiler::front::ast::AST *root, Hash constTbale) {
        std::cout << "开始优化ast" << std::endl;
        //遍历根节点中的Block,建立哈希表
        for (auto node:root->codeBlock) {
            if (node->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(node)->declareList) {
                    switch (subNode->nodetype) {
                        case VarDeclareWithInitType: {
                            static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                            break;
                        }
                        case ConstDeclareType: {
                            static_cast<ConstDeclare *>(subNode)->value = FirstPassExpr(static_cast<ConstDeclare *>(subNode)->value, constTbale);
                            //替换完后加入到hashtable中
                            if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                            }
                            break;
                        }
                    }
                }
            } else if (node->nodetype == FunctionDefineType) {
                FirstPassNode(static_cast<Block *>(static_cast<FunctionDefine *>(node)->body), constTbale);
            } else if (node->nodetype == VoidStatementType) {
                root->codeBlock.remove(node);
            }
        }
    }

    void FirstPassNode(compiler::front::ast::Block *node, Hash constTbale) {
        for (auto item = node->blockItem.begin(); item != node->blockItem.end(); item++) {
            switch ((*item)->nodetype) {
                case DeclareStatementType: {
                    for (auto subNode:static_cast<DeclareStatement *>(*item)->declareList) {
                        switch (subNode->nodetype) {
                            case VarDeclareWithInitType: {
                                static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                                break;
                            }
                            case ConstDeclareType: {
                                static_cast<ConstDeclare *>(subNode)->value = FirstPassExpr(static_cast<ConstDeclare *>(subNode)->value, constTbale);
                                //替换完后加入到hashtable中
                                if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                    constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case AssignStmtType: {
                    static_cast<AssignStmt *>(*item)->rightExpr = FirstPassExpr(static_cast<AssignStmt *>(*item)->rightExpr, constTbale);
                    break;
                }
                case IfStatementType: {
                    static_cast<IfStatement *>(*item)->cond = FirstPassExpr(static_cast<IfStatement *>(*item)->cond, constTbale);
                    if (static_cast<IfStatement *>(*item)->trueBlock->nodetype == BlockType) {
                        FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(*item)->trueBlock), constTbale);
                    } else {
                        FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(*item)->trueBlock), constTbale);
                    }
                    if (static_cast<IfStatement *>(*item)->elseBlock->nodetype == BlockType) {
                        FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(*item)->elseBlock), constTbale);
                    } else {
                        FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(*item)->elseBlock), constTbale);
                    }
                    break;
                }
                case WhileStatementType: {
                    static_cast<WhileStatement *>(*item)->cond = FirstPassExpr(static_cast<IfStatement *>(*item)->cond,
                                                                               constTbale);
                    if (static_cast<WhileStatement *>(*item)->nodetype == BlockType) {
                        FirstPassNode(static_cast<Block *>(static_cast<WhileStatement *>(*item)->loopBlock),
                                      constTbale);
                    } else {
                        FirstPassStmt(static_cast<Stmt *>(static_cast<WhileStatement *>(*item)->loopBlock), constTbale);
                    }
                    break;
                }
                case FunctionCallType: {
                    for (auto i = static_cast<FunctionCall *>(*item)->args->args.begin();
                         i != static_cast<FunctionCall *>(*item)->args->args.end(); i++) {
                        *i = FirstPassExpr(*i, constTbale);
                    }
                    break;
                }
                case ReturnStatementType: {
                    if (static_cast<ReturnStatement *>(*item)->returnExp == NULL) {
                        node->blockItem.erase(item++);
                        item--;
                        break;
                    }
                    static_cast<ReturnStatement *>(*item)->returnExp = FirstPassExpr(
                            static_cast<ReturnStatement *>(*item)->returnExp, constTbale);
                    break;
                }
                case VoidStatementType: {
                    node->blockItem.erase(item++);
                    item--;
                    break;
                }
            }
        }
    }

    void FirstPassStmt(compiler::front::ast::Stmt *stmt, Hash constTbale) {
        switch (stmt->nodetype) {
            case DeclareStatementType: {
                for (auto subNode = static_cast<DeclareStatement *>(stmt)->declareList.begin();
                     subNode != static_cast<DeclareStatement *>(stmt)->declareList.end(); subNode++) {
                    switch ((*subNode)->nodetype) {
                        case VarDeclareWithInitType: {
                            static_cast<VarDeclareWithInit *>(*subNode)->value = FirstPassExpr(
                                    static_cast<VarDeclareWithInit *>(*subNode)->value, constTbale);
                            break;
                        }
                        case ConstDeclareType: {
                            static_cast<VarDeclareWithInit *>(*subNode)->value = FirstPassExpr(
                                    static_cast<VarDeclareWithInit *>(*subNode)->value, constTbale);
                            if (static_cast<ConstDeclare *>(*subNode)->value->nodetype == NumberExpressionType) {
                                constTbale[static_cast<ConstDeclare *>(*subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(*subNode)->value)->value;
                            }
                            break;
                        }
                    }
                }
            }
            case AssignStmtType: {
                static_cast<AssignStmt *>(stmt)->rightExpr = FirstPassExpr(
                        static_cast<AssignStmt *>(stmt)->rightExpr, constTbale);
                break;
            }
            case BlockType:{
                FirstPassNode(static_cast<Block *>(stmt),constTbale);
                break;
            }
            case ReturnStatementType:{
                if (static_cast<ReturnStatement *>(stmt)->returnExp == NULL) {
                    break;
                }
                static_cast<ReturnStatement *>(stmt)->returnExp = FirstPassExpr(static_cast<ReturnStatement *>(stmt)->returnExp, constTbale);
                break;
            }
            case IfStatementType: {
                static_cast<IfStatement *>(stmt)->cond = FirstPassExpr(static_cast<IfStatement *>(stmt)->cond, constTbale);
                if (static_cast<IfStatement *>(stmt)->trueBlock->nodetype == BlockType) {
                    FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(stmt)->trueBlock), constTbale);
                } else {
                    FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(stmt)->trueBlock), constTbale);
                }
                if (static_cast<IfStatement *>(stmt)->elseBlock->nodetype == BlockType) {
                    FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(stmt)->elseBlock), constTbale);
                } else {
                    FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(stmt)->elseBlock), constTbale);
                }
                break;
            }
            case WhileStatementType: {
                static_cast<WhileStatement *>(stmt)->cond = FirstPassExpr(static_cast<IfStatement *>(stmt)->cond,constTbale);
                if (static_cast<WhileStatement *>(stmt)->nodetype == BlockType) {
                    FirstPassNode(static_cast<Block *>(static_cast<WhileStatement *>(stmt)->loopBlock),constTbale);
                } else {
                    FirstPassStmt(static_cast<Stmt *>(static_cast<WhileStatement *>(stmt)->loopBlock), constTbale);
                }
                break;
            }
            default: {
                std::cout << stmt->nodetype<<" ";
                break;
            }
        }
    }

    compiler::front::ast::Expression *FirstPassExpr(compiler::front::ast::Expression *expr, Hash constTbale) {
        switch (expr->nodetype) {
            case IdentifierType: {
                string Name = static_cast<Identifier *>(expr)->name;
                if (constTbale.count(Name) > 0) {
                    delete expr;
                    return new NumberExpression(constTbale[Name]);
                } else {
                    return expr;
                }
                break;
            }
            case FunctionCallType: {
                for (auto i = static_cast<FunctionCall *>(expr)->args->args.begin();
                     i != static_cast<FunctionCall *>(expr)->args->args.end(); i++) {
                    *i = FirstPassExpr(*i, constTbale);
                }
                return expr;
                break;
            }
            case BinaryExpressionType: {
                static_cast<BinaryExpression *>(expr)->rightExpr = FirstPassExpr(
                        static_cast<BinaryExpression *>(expr)->rightExpr, constTbale);
                static_cast<BinaryExpression *>(expr)->leftExpr = FirstPassExpr(
                        static_cast<BinaryExpression *>(expr)->leftExpr, constTbale);
                return expr;
                break;
            }
            case UnaryExpressionType: {
                static_cast<UnaryExpression *>(expr)->right = FirstPassExpr(static_cast<UnaryExpression *>(expr)->right,
                                                                            constTbale);
                return expr;
                break;
            }
            default: {
                return expr;
            }
        }
    }
}