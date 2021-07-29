//
// Created by karl on 2021/7/7.
//
#include "astpass.h"

using namespace compiler::front::ast;

namespace compiler::astpassir {
    bool caluExpersion(compiler::front::ast::Expression *exp, int &result) {
        switch (exp->nodetype) {
            case NumberExpressionType:
                result = static_cast<NumberExpression *>(exp)->value;
                return true;
            case IdentifierType:
                return false;
            case BinaryExpressionType: {
                auto exprTemp = static_cast<BinaryExpression *>(exp);
                int left;
                int right;
                int leftCan = caluExpersion(exprTemp->leftExpr, left);
                int rightCan = caluExpersion(exprTemp->rightExpr, right);
                if (leftCan && rightCan) {
                    delete exprTemp->leftExpr;
                    exprTemp->leftExpr = NULL;
                    delete exprTemp->rightExpr;
                    exprTemp->rightExpr = NULL;
                    switch (exprTemp->op) {
                        case ADD: {
                            result = left + right;
                            return true;
                        }
                        case SUB: {
                            result = left - right;
                            return true;
                        }
                        case MUL: {
                            result = left * right;
                            return true;
                        }
                        case DIV: {
                            result = left / right;
                            return true;
                        }
                        case MOD: {
                            result = left % right;
                            return true;
                        }
                        case EQ: {
                            result = left == right;
                            return true;
                        }
                        case NE: {
                            result = left != right;
                            return true;
                        }
                        case LT: {
                            result = left < right;
                            return true;
                        }
                        case LE: {
                            result = left <= right;
                            return true;
                        }
                        case GT: {
                            result = left > right;
                            return true;
                        }
                        case GE: {
                            result = left >= right;
                            return true;
                        }
                        case AND_OP: {
                            result = left && right;
                            return true;
                        }
                        case OR_OP: {
                            result = left || right;
                            return true;
                        }
                        case NOT_EQUAL: {
                            result = left != right;
                            return true;
                        }
                    }
                } else if (exprTemp->op == AND_OP && ((leftCan && left == 0) || (rightCan && right == 0))) {
                    delete exprTemp->leftExpr;
                    exprTemp->leftExpr = NULL;
                    delete exprTemp->rightExpr;
                    exprTemp->leftExpr = NULL;
                    result = 0;
                    return true;
                } else if (exprTemp->op == OR_OP && ((leftCan && left != 0) || (rightCan && right != 0))) {
                    delete exprTemp->leftExpr;
                    exprTemp->leftExpr = NULL;
                    delete exprTemp->rightExpr;
                    exprTemp->rightExpr = NULL;
                    result = 1;
                    return true;
                } else {
                    return false;
                }
            }
            case UnaryExpressionType: {
                auto exprTemp = static_cast<UnaryExpression *>(exp);
                int res;
                int resCan = caluExpersion(exprTemp->right, res);
                if (resCan) {
                    delete exprTemp->right;
                    exprTemp->right = NULL;
                    switch (exprTemp->op) {
                        case ADD: {
                            result = res;
                            return true;
                        }
                        case SUB: {
                            result = -res;
                            return true;
                        }
                        case NOT_EQUAL: {
                            result = !res;
                            return true;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
            }
            default: {
                return false;
            }
        }
    }

    void FirstPassRoot(compiler::front::ast::AST *root, Hash constTbale) {
        std::cout << "开始优化ast" << std::endl;
        //遍历根节点中的Block,建立哈希表
        for (auto node:root->codeBlock) {
            if (node->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(node)->declareList) {
                    switch (subNode->nodetype) {
                        case ArrayDeclareWithInitType: {
                            FirstPassArray(subNode, constTbale);
                            break;
                        }
                        case ConstArrayType: {
                            FirstPassArray(subNode, constTbale);
                            break;
                        }
                        case VarDeclareWithInitType: {
                            static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                            int result;
                            if (caluExpersion(static_cast<VarDeclareWithInit *>(subNode)->value, result)) {
                                delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(result);
                            }
                            break;
                        }
                        case ConstDeclareType: {
                            static_cast<ConstDeclare *>(subNode)->value = FirstPassExpr(static_cast<ConstDeclare *>(subNode)->value, constTbale);
                            //替换完后加入到hashtable中
                            int result;
                            if (caluExpersion(static_cast<ConstDeclare *>(subNode)->value, result)) {
                                delete static_cast<ConstDeclare *>(subNode)->value;
                                static_cast<ConstDeclare *>(subNode)->value = new NumberExpression(result);
                            }

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
                            case ArrayDeclareWithInitType: {
                                FirstPassArray(subNode, constTbale);
                                break;
                            }
                            case ConstArrayType: {
                                FirstPassArray(subNode, constTbale);
                                break;
                            }
                            case VarDeclareWithInitType: {
                                static_cast<VarDeclareWithInit *>(subNode)->value = FirstPassExpr(static_cast<VarDeclareWithInit *>(subNode)->value, constTbale);
                                int result;
                                if (caluExpersion(static_cast<VarDeclareWithInit *>(subNode)->value, result)) {
                                    delete static_cast<VarDeclareWithInit *>(subNode)->value;
                                    static_cast<VarDeclareWithInit *>(subNode)->value = new NumberExpression(result);
                                }
                                break;
                            }
                            case ConstDeclareType: {
                                static_cast<ConstDeclare *>(subNode)->value = FirstPassExpr(static_cast<ConstDeclare *>(subNode)->value, constTbale);
                                int result;
                                if (caluExpersion(static_cast<ConstDeclare *>(subNode)->value, result)) {
                                    delete static_cast<ConstDeclare *>(subNode)->value;
                                    static_cast<ConstDeclare *>(subNode)->value = new NumberExpression(result);
                                }
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
                    static_cast<ReturnStatement *>(*item)->returnExp = FirstPassExpr(static_cast<ReturnStatement *>(*item)->returnExp, constTbale);
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
                        case ArrayDeclareWithInitType: {
                            FirstPassArray(*subNode, constTbale);
                            break;
                        }
                        case ConstArrayType: {
                            FirstPassArray(*subNode, constTbale);
                            break;
                        }
                        case VarDeclareWithInitType: {
                            static_cast<VarDeclareWithInit *>(*subNode)->value = FirstPassExpr(static_cast<VarDeclareWithInit *>(*subNode)->value, constTbale);
                            break;
                        }
                        case ConstDeclareType: {
                            static_cast<VarDeclareWithInit *>(*subNode)->value = FirstPassExpr(static_cast<VarDeclareWithInit *>(*subNode)->value, constTbale);
                            if (static_cast<ConstDeclare *>(*subNode)->value->nodetype == NumberExpressionType) {
                                constTbale[static_cast<ConstDeclare *>(*subNode)->name->name] = static_cast<NumberExpression *>(static_cast<ConstDeclare *>(*subNode)->value)->value;
                            }
                            break;
                        }
                    }
                }
            }
            case AssignStmtType: {
                static_cast<AssignStmt *>(stmt)->rightExpr = FirstPassExpr(static_cast<AssignStmt *>(stmt)->rightExpr, constTbale);
                break;
            }
            case BlockType: {
                FirstPassNode(static_cast<Block *>(stmt), constTbale);
                break;
            }
            case ReturnStatementType: {
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
                static_cast<WhileStatement *>(stmt)->cond = FirstPassExpr(static_cast<IfStatement *>(stmt)->cond, constTbale);
                if (static_cast<WhileStatement *>(stmt)->nodetype == BlockType) {
                    FirstPassNode(static_cast<Block *>(static_cast<WhileStatement *>(stmt)->loopBlock), constTbale);
                } else {
                    FirstPassStmt(static_cast<Stmt *>(static_cast<WhileStatement *>(stmt)->loopBlock), constTbale);
                }
                break;
            }
            default: {
                std::cout << stmt->nodetype << " ";
                break;
            }
        }
    }

    void FirstPassArray(compiler::front::ast::Declare *array, Hash constTbale) {
        switch (array->nodetype) {
            case ArrayDeclareWithInitType: {
                auto Id = static_cast<ArrayIdentifier *>(static_cast<ArrayDeclareWithInit *>(array)->name);
                for (auto i = Id->index.begin(); i != Id->index.end(); i++) {
                    (*i) = FirstPassExpr((*i), constTbale);
                    int result;
                    if (caluExpersion((*i), result)) {
                        delete (*i);
                        (*i) = new NumberExpression(result);
                    } else {
                        std::cout << "数组下标志不能计算";
                    }
                }
                //数组线性化
                static_cast<ArrayDeclareWithInit *>(array)->initVal->initValList = FirstPassArrayLinelize(0, Id->index, static_cast<ArrayDeclareWithInit *>(array)->initVal->initValList);
                break;
            }
            case ConstArrayType: {
                auto Id = static_cast<ArrayIdentifier *>(static_cast<ConstArray *>(array)->name);
                for (auto i = Id->index.begin(); i != Id->index.end(); i++) {
                    (*i) = FirstPassExpr((*i), constTbale);
                    int result;
                    if (caluExpersion((*i), result)) {
                        delete (*i);
                        (*i) = new NumberExpression(result);
                    } else {
                        std::cout << "数组下标志不能计算";
                    }
                }
                static_cast<ConstArray *>(array)->initVal->initValList = FirstPassArrayLinelize(0, Id->index, static_cast<ConstArray *>(array)->initVal->initValList);
                break;
            }
            case ArrayDeclareType: {
                auto Id = static_cast<ArrayIdentifier *>(static_cast<ConstArray *>(array)->name);
                for (auto i = Id->index.begin(); i != Id->index.end(); i++) {
                    (*i) = FirstPassExpr((*i), constTbale);
                    int result;
                    if (caluExpersion((*i), result)) {
                        delete (*i);
                        (*i) = new NumberExpression(result);
                    } else {
                        std::cout << "数组下标志不能计算";
                    }
                }
                break;
            }
        }
    }

    vector<compiler::front::ast::Expression *> FirstPassArrayLinelize(int sizeIndex, vector<compiler::front::ast::Expression *> &index, vector<compiler::front::ast::Expression *> &values) {
        vector<Expression *> data;
        int needSize = 1;
        int count = 0;
        if (sizeIndex + 1 < index.size()) {
            for (int i = sizeIndex; i < index.size() - 1; i++) {
                needSize = needSize * static_cast<NumberExpression *>(index[i + 1])->value;
            }
            for (int j = 0; j < values.size(); j++) {
                if (values[j]->nodetype == ArrayInitValType) {
                    vector<Expression *> temp = FirstPassArrayLinelize(sizeIndex + 1, index, static_cast<ArrayInitVal *>(values[j])->initValList);
                    while (temp.size() < needSize) {
                        temp.push_back(new NumberExpression(0));
                    }
                    data.insert(data.end(), temp.begin(), temp.end());
                } else {
                    data.push_back(values[j]);
                    count = count + 1;
                    count = count % (needSize + 1);
                    if (count < needSize - 1 && (j + 1 == values.size() || values[j + 1]->nodetype == ArrayInitValType)) {
                        while (count < needSize) {
                            data.push_back(new NumberExpression(0));
                            count++;
                        }
                        count = 0;
                    }
                }
            }
            while (needSize * static_cast<NumberExpression *>(index[sizeIndex])->value > data.size()) {
                data.push_back(new NumberExpression(0));
            }
        } else {
            data = values;
            while (static_cast<NumberExpression *>(index[sizeIndex])->value > data.size()) {
                data.push_back(new NumberExpression(0));
            }
        }
        return data;
    }

    compiler::front::ast::Expression *FirstPassExpr(compiler::front::ast::Expression *expr, Hash constTbale) {
        switch (expr->nodetype) {
            case ArrayIdentifierType: {
                auto Id = static_cast<ArrayIdentifier *>(expr);
                for (auto i = Id->index.begin(); i != Id->index.end(); i++) {
                    (*i) = FirstPassExpr((*i), constTbale);
                    int result;
                    if (caluExpersion((*i), result)) {
                        delete (*i);
                        (*i) = new NumberExpression(result);
                    } else {
                        std::cout << "数组下标志不能计算";
                    }
                }
                break;
            }
            case IdentifierType: {
                string Name = static_cast<Identifier *>(expr)->name;
                if (constTbale.count(Name) > 0) {
                    delete expr;
                    return new NumberExpression(constTbale[Name]);
                }
                break;
            }
            case FunctionCallType: {
                for (auto i = static_cast<FunctionCall *>(expr)->args->args.begin(); i != static_cast<FunctionCall *>(expr)->args->args.end(); i++) {
                    *i = FirstPassExpr(*i, constTbale);
                }
                break;
            }
            case BinaryExpressionType: {
                static_cast<BinaryExpression *>(expr)->rightExpr = FirstPassExpr(static_cast<BinaryExpression *>(expr)->rightExpr, constTbale);
                static_cast<BinaryExpression *>(expr)->leftExpr = FirstPassExpr(static_cast<BinaryExpression *>(expr)->leftExpr, constTbale);
                break;
            }
            case UnaryExpressionType: {
                static_cast<UnaryExpression *>(expr)->right = FirstPassExpr(static_cast<UnaryExpression *>(expr)->right, constTbale);
                break;
            }
        }
        int result;
        if (caluExpersion(expr, result)) {
            delete expr;
            return new NumberExpression(result);
        } else {
            return expr;
        }
    }
}

