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
    }

    void FirstPassNode(compiler::front::ast::FunctionDefine *node, Hash constTbale) {
        for (auto blkitem:node->body->blockItem) {
            if (blkitem->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(blkitem)->declareList) {
                    switch (subNode->nodetype) {
                        case (ConstDeclareType): {
                            if (static_cast<ConstDeclare *>(subNode)->value->nodetype == NumberExpressionType) {
                                constTbale[static_cast<ConstDeclare *>(subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(subNode)->value)->value;
                            }
                        }
                    }
                }
            }
        }
    }
}
