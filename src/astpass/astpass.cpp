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
                            if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == IdentifierType) {
                                string constName = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                if(constTbale.count(constName)>0)
                                {
                                    delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                    static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(
                                            constTbale[constName]);
                                }
                            }
                            break;
                        }
                        case ConstDeclareType: {
                            if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == IdentifierType) {
                                string constName = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                if(constTbale.count(constName)>0)
                                {
                                    delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                    static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(
                                            constTbale[constName]);
                                }
                            }
                            break;
                        }
                    }
                }
            }
            if (node->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(node)->declareList) {
                    if (subNode->nodetype == ConstDeclareType) {
                        if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                            constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                        }
                    }
                }
            } else if (node->nodetype == FunctionDefineType) {
                FirstPassNode(static_cast<Block *>(static_cast<FunctionDefine *>(node)->body), constTbale);
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
                                if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == IdentifierType) {
                                    string constName = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                    if(constTbale.count(constName)>0)
                                    {
                                        delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                        static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(
                                                constTbale[constName]);
                                    }
                                }
                                break;
                            }
                            case ConstDeclareType: {
                                if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == IdentifierType) {
                                    string constName = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                    if(constTbale.count(constName)>0)
                                    {
                                        delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                        static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(
                                                constTbale[constName]);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                case AssignStmtType: {
                    if (static_cast<AssignStmt *>(item)->rightExpr->nodetype == IdentifierType) {
                        string rightName = static_cast<Identifier *>(static_cast<AssignStmt *>(item)->rightExpr)->name;
                        if(constTbale.count(rightName)>0)
                        {
                            delete static_cast<AssignStmt *>(item)->rightExpr;
                            static_cast<AssignStmt *>(item)->rightExpr = new NumberExpression(constTbale[rightName]);
                        }
                    }
                    break;
                }
                case FunctionCallType:
                {
                    for(auto i = static_cast<FunctionCall*>(item)->args->args.begin(); i!=static_cast<FunctionCall*>(item)->args->args.end();i++)
                    {
                        if((*i)->nodetype==IdentifierType)
                        {
                            string Name = static_cast<Identifier *>(*i)->name;
                            if(constTbale.count(Name)>0)
                            {
                                (*i) = new NumberExpression(constTbale[Name]);
                            }
                        }
                    }
                    /*for(int i=0; i<static_cast<FunctionCall*>(item)->args->args.size(); i++)
                    {
                        if(static_cast<FunctionCall*>(item)->args->args[i]->nodetype==IdentifierType)
                        {
                            string Name = static_cast<Identifier *>(static_cast<FunctionCall*>(item)->args->args[i])->name;
                            if(constTbale.count(Name)>0)
                            {
                                static_cast<FunctionCall*>(item)->args->args[i] = new NumberExpression(constTbale[Name]);
                            }
                        }
                    }*/
                }
                case CommaExpressionType:
                {

                }
            }
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
            }
        }
    }

    void FirstPassStmt(compiler::front::ast::Stmt *stmt, Hash constTbale) {
        switch (stmt->nodetype) {
            case DeclareStatementType: {
                for (auto subNode:static_cast<DeclareStatement *>(stmt)->declareList) {
                    switch (subNode->nodetype) {
                        case VarDeclareWithInitType: {
                            if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == IdentifierType) {
                                string constName = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                string name = static_cast<VarDeclareWithInit *>(subNode)->name->name;
                                static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(
                                        constTbale[constName]);
                            }
                            break;
                        }
                        case ConstDeclareType: {
                            if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == IdentifierType) {
                                string constName = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                string name = static_cast<VarDeclareWithInit *>(subNode)->name->name;
                                static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(
                                        constTbale[constName]);
                            }
                            break;
                        }
                    }
                }
            }
            case AssignStmtType: {
                if (static_cast<AssignStmt *>(stmt)->rightExpr->nodetype == IdentifierType) {
                    string rightName = static_cast<Identifier *>(static_cast<AssignStmt *>(stmt)->rightExpr)->name;
                    delete static_cast<AssignStmt *>(stmt)->rightExpr;
                    static_cast<AssignStmt *>(stmt)->rightExpr = new NumberExpression(constTbale[rightName]);
                }
                break;
            }
        }
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

    void FirstPassExpr(compiler::front::ast::Expression *expr, Hash constTbale) {

    }
}


