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
                            static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(
                                    static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                            break;
                        }
                        case ConstDeclareType: {
                            static_cast<ConstDeclare *>(subNode)->value = FirstPassExpr(
                                    static_cast<ConstDeclare *>(subNode)->value, constTbale);
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
            }else if(node->nodetype==VoidStatementType)
            {
                root->codeBlock.remove(node);
            }
        }
    }

    void FirstPassNode(compiler::front::ast::Block *node, Hash constTbale) {
        for (auto item:node->blockItem) {
            switch (item->nodetype) {
                case DeclareStatementType: {
                    for (auto subNode:static_cast<DeclareStatement *>(item)->declareList) {
                        switch (subNode->nodetype) {
                            case VarDeclareWithInitType: {
                                static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(
                                        static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                                break;
                            }
                            case ConstDeclareType: {
                                static_cast<ConstDeclare *>(subNode)->value = FirstPassExpr(
                                        static_cast<ConstDeclare *>(subNode)->value, constTbale);
                                //替换完后加入到hashtable中
                                if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                    constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                                }
                                break;
                            }
                        }
                    }
                }
                case AssignStmtType: {
                    static_cast<AssignStmt *>(item)->rightExpr = FirstPassExpr(
                            static_cast<AssignStmt *>(item)->rightExpr, constTbale);
                    break;
                }
                case IfStatementType: {
                    FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(item)->trueBlock), constTbale);
                    if (static_cast<IfStatement *>(item)->elseBlock->nodetype == BlockType) {
                        FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(item)->elseBlock), constTbale);
                    } else {
                        FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(item)->elseBlock),
                                      constTbale);
                    }
                    break;
                }
                case WhileStatementType: {
                    FirstPassNode(static_cast<Block *>(static_cast<WhileStatement *>(item)->loopBlock), constTbale);
                    break;
                }
                case FunctionCallType: {
                    for (auto i = static_cast<FunctionCall *>(item)->args->args.begin();
                         i != static_cast<FunctionCall *>(item)->args->args.end(); i++) {
                        *i = FirstPassExpr(*i, constTbale);
                    }
                }
                case ReturnStatementType: {
                    static_cast<ReturnStatement*>(item)->returnExp = FirstPassExpr(static_cast<ReturnStatement*>(item)->returnExp,constTbale);
                }
            }
            /*//添加到table
            switch (item->nodetype) {
                case DeclareStatementType: {
                    for (auto subNode:static_cast<DeclareStatement *>(item)->declareList) {
                        switch (subNode->nodetype) {
                            case ConstDeclareType: {
                                if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                    constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case IfStatementType: {
                    FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(item)->trueBlock), constTbale);
                    if (static_cast<IfStatement *>(item)->elseBlock->nodetype == BlockType) {
                        FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(item)->elseBlock), constTbale);
                    } else {
                        FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(item)->elseBlock),
                                      constTbale);
                    }
                    break;
                }
                case WhileStatementType: {
                    FirstPassNode(static_cast<Block *>(static_cast<WhileStatement *>(item)->loopBlock), constTbale);
                    break;
                }
            }*/
        }
    }

    void FirstPassStmt(compiler::front::ast::Stmt *stmt, Hash constTbale) {
        switch (stmt->nodetype) {
            case DeclareStatementType: {
                for (auto subNode:static_cast<DeclareStatement *>(stmt)->declareList) {
                    switch (subNode->nodetype) {
                        case VarDeclareWithInitType: {
                            static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(
                                    static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                            break;
                        }
                        case ConstDeclareType: {
                            static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(
                                    static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
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
        }
        //添加table
        switch (stmt->nodetype) {
            case DeclareStatementType: {
                for (auto subNode:static_cast<DeclareStatement *>(stmt)->declareList) {
                    switch (subNode->nodetype) {
                        case ConstDeclareType: {
                            if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                            }
                            break;
                        }
                    }
                }
                break;
            }
            case IfStatementType: {
                FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(stmt)->trueBlock), constTbale);
                if (static_cast<IfStatement *>(stmt)->elseBlock->nodetype == BlockType) {
                    FirstPassNode(static_cast<Block *>(static_cast<IfStatement *>(stmt)->elseBlock), constTbale);
                } else {
                    FirstPassStmt(static_cast<IfStatement *>(static_cast<IfStatement *>(stmt)->elseBlock),
                                  constTbale);
                }
                break;
            }
            case WhileStatementType: {
                FirstPassNode(static_cast<Block *>(static_cast<WhileStatement *>(stmt)->loopBlock), constTbale);
                break;
            }
            default: {
                std::cout << stmt->nodetype;
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
            }
            case FunctionCallType: {
                for (auto i = static_cast<FunctionCall *>(expr)->args->args.begin();
                     i != static_cast<FunctionCall *>(expr)->args->args.end(); i++) {
                    *i = FirstPassExpr(*i, constTbale);
                }
            }
            default: {
                return expr;
            }
        }
    }
}