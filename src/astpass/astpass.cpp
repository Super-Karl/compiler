//
// Created by karl on 2021/7/7.
//
#include "astpass.h"
#include <unordered_map>
#include <iostream>


using namespace compiler::front::ast;

namespace compiler::astpassir {
    int caluExpersion(Node *exp, Hash constTbale) {
        switch (exp->nodetype) {
            case NumberExpressionType:
                return static_cast<NumberExpression *>(exp)->value;
            case IdentifierType:
                return constTbale.at(static_cast<Identifier *>(exp)->name);
        }
    }

    void FirstPassRoot(compiler::front::ast::AST *root, Hash constTbale) {
        std::cout << "开始优化ast" << std::endl;
        //遍历根节点中的Block,建立哈希表
        for (auto node:root->codeBlock) {
            if (node->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(node)->declareList) {
                    if (subNode->nodetype == ConstDeclareType) {
                        if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                            constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                        }
                    }
                }
            } else if (node->nodetype == FunctionDefineType) {
                FirstPassNode(static_cast<FunctionDefine *>(node), constTbale);
            }
        }
        //遍历Block,替换Const
        for (auto node:root->codeBlock) {
            if (node->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(node)->declareList) {
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
        }
    }

    void FirstPassNode(compiler::front::ast::FunctionDefine *node, Hash constTbale) {
        for (auto blkitem:node->body->blockItem) {
            if (blkitem->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(blkitem)->declareList) {
                    switch (subNode->nodetype) {
                        case ConstDeclareType: {
                            if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                            }
                            break;
                        }
                    }
                }
            }
        }
        for (auto blkitem:node->body->blockItem) {
            if (blkitem->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(blkitem)->declareList) {
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
        }
    }
}
