//
// Created by karl on 2021/7/29.
//
#include "generateBack.h"

using namespace compiler::front::ast;
namespace compiler::back {

    class VAR {
    public:
        string name;
        int value;
        int count;
        int index;

        VAR(string name, int value, int index) : index(index), name(name), value(value) {}
    };

    //变量
    std::unordered_map<string, std::unordered_map<string, VAR *>> vartable;

    std::unordered_map<string, int> tableIndex;

    string nowfunc;

    void printASM(list<compiler::back::INS *> &list) {
        for (auto i:list) {
            i->print();
        }
    }


    list<compiler::back::INS *> generateBack(compiler::front::ast::AST *root) {
        cout << "----------------------------------\n"
                "开始生成arm汇编\n"
                "----------------------------------\n";
        list<INS *> backlist;
        backlist.push_back(new MACRO());
        for (auto block:root->codeBlock) {
            if (block->nodetype == FunctionDefineType) {
                FunctionDefine *func = static_cast<FunctionDefine *>(block);
                nowfunc = func->name->name;
                tableIndex[nowfunc] = 0;
                backlist.push_back(new FUNC(func->name->name));
                backlist.push_back(new STMDB());
                //生成函数back
                generateBackFunction(backlist, func);
                backlist.push_back(new LDMIA());
            } else if (block->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(block)->declareList) {
                    switch (subNode->nodetype) {
                        case VarDeclareWithInitType: {
                            int value = 0;
                            string name = subNode->name->name;
                            if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == NumberExpressionType) {
                                value = static_cast<NumberExpression *>(static_cast<VarDeclareWithInit *>(subNode)->value)->value;
                            }
                            int index = tableIndex["global"];
                            vartable["global"][name] = new VAR(name, value, index);
                            tableIndex["global"]++;
                            backlist.push_back(new GLOBAL(name, value));
                            break;
                        }
                        case VarDeclareType: {
                            string name = subNode->name->name;
                            int index = tableIndex["global"];
                            vartable["global"][name] = new VAR(name, 0, index);
                            tableIndex["global"]++;
                            backlist.push_back(new GLOBAL(name, 0));
                            break;
                        }
                    }
                }
            }
        }
        return backlist;
    }

    void generateBackFunction(list<INS *> &backlist, compiler::front::ast::FunctionDefine *func) {
        for (auto item = func->body->blockItem.begin(); item != func->body->blockItem.end(); item++) {
            switch ((*item)->nodetype) {
                case DeclareStatementType: {
                    for (auto subNode:static_cast<DeclareStatement *>(*item)->declareList) {
                        switch (subNode->nodetype) {
                            case VarDeclareWithInitType: {
                                string name = subNode->name->name;
                                int index = tableIndex[nowfunc];
                                vartable[nowfunc][name] = new VAR(name, 0, index);
                                tableIndex[nowfunc]++;
                                //把右值放到r2
                                switch (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype) {
                                    case NumberExpressionType: {
                                        //右值为数字
                                        int value = 0;
                                        value = static_cast<NumberExpression *>(static_cast<VarDeclareWithInit *>(subNode)->value)->value;
                                        if (value < 65535) {
                                            backlist.push_back(new MOV(2, value));
                                        } else {
                                            backlist.push_back(new MOV32(2, value));
                                        }
                                        break;
                                    }
                                    case IdentifierType: {
                                        //右值为单个字母
                                        string name = static_cast<Identifier *>(static_cast<VarDeclareWithInit *>(subNode)->value)->name;
                                        int isglobal = vartable[nowfunc].count(name) == 0;
                                        if (isglobal) {
                                            //全局变量
                                            //取地址到r4
                                            backlist.push_back(new MOV32(4, "gv_" + name));
                                            //读到r2
                                            backlist.push_back(new LDR(2, address("r4", 0)));
                                        } else {
                                            //读到r2
                                            backlist.push_back(new LDR(2, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                                        }
                                        break;
                                    }
                                    case BinaryExpressionType: {
                                        //右值为BinaryExpression，计算后将计算后的值放在r2
                                        generateBinaryExpression(backlist, static_cast<BinaryExpression *>(static_cast<VarDeclareWithInit *>(subNode)->value), 0);
                                        break;
                                    }
                                }
                                //存到内存中
                                backlist.push_back(new STR(2, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                                break;
                            }
                            case VarDeclareType: {
                                string name = subNode->name->name;
                                int index = tableIndex[nowfunc];
                                vartable[nowfunc][name] = new VAR(name, 0, index);
                                tableIndex[nowfunc]++;
                                backlist.push_back(new STR(0, address("sp", -4, "!")));
                                break;
                            }
                        }
                    }
                    break;
                }
                case AssignStmtType: {
                    string name = static_cast<AssignStmt *>(*item)->name->name;
                    switch (static_cast<AssignStmt *>(*item)->rightExpr->nodetype) {
                        //把右值放到r2
                        case NumberExpressionType: {
                            //右值为数字
                            int value = 0;
                            value = static_cast<NumberExpression *>(static_cast<AssignStmt *>(*item)->rightExpr)->value;
                            if (value < 65535) {
                                backlist.push_back(new MOV(2, value));
                            } else {
                                backlist.push_back(new MOV32(2, value));
                            }
                            break;
                        }
                        case IdentifierType: {
                            //右值为单个字母
                            string name = static_cast<Identifier *>(static_cast<AssignStmt *>(*item)->rightExpr)->name;
                            int isglobal = vartable[nowfunc].count(name) == 0;
                            if (isglobal) {
                                //全局变量
                                //取地址到r4
                                backlist.push_back(new MOV32(4, "gv_" + name));
                                //读到r2
                                backlist.push_back(new LDR(2, address("r4", 0)));
                            } else {
                                backlist.push_back(new LDR(2, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                            }
                            break;
                        }
                        case BinaryExpressionType: {
                            //右值为BinaryExpression，计算后将计算后的值放在r2
                            generateBinaryExpression(backlist, static_cast<BinaryExpression *>(static_cast<AssignStmt *>(*item)->rightExpr), 0);
                            break;
                        }
                    }

                    //判断全局变量还是局部变量
                    int isglobal = vartable[nowfunc].count(name) == 0;
                    if (isglobal) {
                        //全局变量
                        //取地址到r3
                        backlist.push_back(new MOV32(3, "gv_" + name));
                        backlist.push_back(new STR(2, address("r3", 0)));
                    } else {
                        backlist.push_back(new STR(2, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                    }
                    break;
                }
                case ReturnStatementType: {
                    switch (static_cast<ReturnStatement *>((*item))->returnExp->nodetype) {
                        case IdentifierType: {
                            string name = static_cast<Identifier *>(static_cast<ReturnStatement *>((*item))->returnExp)->name;
                            int isglobal = vartable[nowfunc].count(name) == 0;
                            if (isglobal) {
                                //全局变量
                                //取地址到r4
                                backlist.push_back(new MOV32(4, "gv_" + name));
                                //读到r3
                                backlist.push_back(new LDR(0, address("r4", 0)));
                            } else {
                                backlist.push_back(new LDR(0, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                            }
                            break;
                        }
                        case NumberExpressionType: {
                            int value = static_cast<NumberExpression *>(static_cast<ReturnStatement *>((*item))->returnExp)->value;
                            backlist.push_back(new MOV(0, value));
                            break;
                        }
                        case BinaryExpressionType: {
                            //右值为BinaryExpression，计算后将计算后的值放在r2
                            generateBinaryExpression(backlist, static_cast<BinaryExpression *>(static_cast<ReturnStatement *>((*item))->returnExp), 0);
                            backlist.push_back(new MOV(0,2,"reg2reg"));
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    void generateBinaryExpression(list<INS *> &backlist, compiler::front::ast::BinaryExpression *expression, int pos) {
        int reg1;
        int reg2;//最终结果
        int reg3;
        if (pos == -1) {
            reg1 = 1;
            reg2 = 1;
            reg3 = 2;
        }
        if (pos == 0) {
            reg1 = 1;
            reg2 = 2;
            reg3 = 3;
        }
        if (pos == 1) {
            reg1 = 2;
            reg2 = 3;
            reg3 = 3;
        }
        //处理左侧
        switch (expression->leftExpr->nodetype) {
            case NumberExpressionType:
            {
                int value = static_cast<NumberExpression*>(expression->leftExpr)->value;
                //TODO 小于0的情况没有考虑
                if (value < 65535) {
                    backlist.push_back(new MOV(reg1, value));
                } else {
                    backlist.push_back(new MOV32(reg1, value));
                }
                break;
            }
            case IdentifierType:
            {
                //右值为单个字母
                string name = static_cast<Identifier *>(expression->leftExpr)->name;
                int isglobal = vartable[nowfunc].count(name) == 0;
                if (isglobal) {
                    //全局变量
                    //取地址到r4
                    backlist.push_back(new MOV32(4, "gv_" + name));
                    //读到r2
                    backlist.push_back(new LDR(reg1, address("r4", 0)));
                } else {
                    backlist.push_back(new LDR(reg1, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                }
                break;
            }
            case BinaryExpressionType:
            {
                generateBinaryExpression(backlist, static_cast<BinaryExpression*>(expression->leftExpr), -1);
                break;
            }
        }
        //处理右侧
        switch (expression->rightExpr->nodetype) {
            case NumberExpressionType:
            {
                int value = static_cast<NumberExpression*>(expression->rightExpr)->value;
                //TODO 小于0的情况没有考虑
                if (value < 65535) {
                    backlist.push_back(new MOV(reg3, value));
                } else {
                    backlist.push_back(new MOV32(reg3, value));
                }
                break;
            }
            case IdentifierType:
            {
                //右值为单个字母
                string name = static_cast<Identifier *>(expression->rightExpr)->name;
                int isglobal = vartable[nowfunc].count(name) == 0;
                if (isglobal) {
                    //全局变量
                    //取地址到r4
                    backlist.push_back(new MOV32(4, "gv_" + name));
                    //读到r3
                    backlist.push_back(new LDR(reg3, address("r4", 0)));
                } else {
                    backlist.push_back(new LDR(reg3, address("fp", -4 - 4 * vartable[nowfunc][name]->index)));
                }
                break;
            }
            case BinaryExpressionType:
            {
                generateBinaryExpression(backlist, static_cast<BinaryExpression*>(expression->rightExpr), 1);
                break;
            }
        }
        switch(expression->op)
        {
            case ADD: {
                backlist.push_back(new OP("add",reg2,reg1,reg3));
                break;
            }
            /*case SUB: {
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
            }*/
        }
    }
}

