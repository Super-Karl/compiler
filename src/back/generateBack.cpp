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
        vector<int> arrayIndex;
        vector<int> arrayValue;

        VAR(string name, int value, int index) : index(index), name(name), value(value) {}
    };

    //变量
    std::unordered_map<string, VAR *> globalVartable;

    int tableIndex = 0;//一个函数的参数个数，用于编号

    string nowfunc;

    int ifStatCount = 0;

    int tableFind(vector<VAR> &vartable, string name) {
        for (int i = vartable.size() - 1; i >= 0; i--) {
            if (vartable[i].name == name) {
                return i;
            }
        }
        return -1;
    }

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
                backlist.push_back(new FUNC(func->name->name));
                backlist.push_back(new STMDB());
                //生成函数back
                vector<VAR> vartable;//记录当前代码块的变量
                ifStatCount = 0;
                generateBackFunction(vartable, backlist, func);
                backlist.push_back(new LDMIA());
            } else if (block->nodetype == DeclareStatementType) {
                for (auto subNode:static_cast<DeclareStatement *>(block)->declareList) {
                    switch (subNode->nodetype) {
                        case ConstArrayType:
                        case ArrayDeclareType: {
                            string name = subNode->name->name;
                            int index = globalVartable.size();
                            VAR *temp = new VAR(name, 0, index);
                            for (auto index:static_cast<ArrayIdentifier *>(subNode->name)->index) {
                                temp->arrayIndex.push_back(static_cast<NumberExpression *>(index)->value);
                            }
                            for (auto value:static_cast<ArrayDeclare *>(subNode)->initVal->initValList) {
                                temp->arrayValue.push_back(static_cast<NumberExpression *>(value)->value);
                            }
                            globalVartable[name] = temp;
                            backlist.push_back(new GLOBALARRAY(name, temp->arrayValue));
                            break;
                        }
                        case ArrayDeclareWithInitType: {
                            string name = subNode->name->name;
                            int index = globalVartable.size();
                            VAR *temp = new VAR(name, 0, index);
                            for (auto index:static_cast<ArrayIdentifier *>(subNode->name)->index) {
                                temp->arrayIndex.push_back(static_cast<NumberExpression *>(index)->value);
                            }
                            for (auto value:static_cast<ArrayDeclareWithInit *>(subNode)->initVal->initValList) {
                                temp->arrayValue.push_back(static_cast<NumberExpression *>(value)->value);
                            }
                            globalVartable[name] = temp;
                            backlist.push_back(new GLOBALARRAY(name, temp->arrayValue));
                            break;
                        }
                        case VarDeclareWithInitType: {
                            int value = 0;
                            string name = subNode->name->name;
                            if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == NumberExpressionType) {
                                value = static_cast<NumberExpression *>(static_cast<VarDeclareWithInit *>(subNode)->value)->value;
                            }
                            int index = globalVartable.size();
                            globalVartable[name] = new VAR(name, value, index);
                            backlist.push_back(new GLOBAL(name, value));
                            break;
                        }
                        case VarDeclareType: {
                            string name = subNode->name->name;
                            int index = globalVartable.size();
                            globalVartable[name] = new VAR(name, 0, index);
                            backlist.push_back(new GLOBAL(name, 0));
                            break;
                        }
                    }
                }
            }
        }
        return backlist;
    }

    void generateBackFunction(vector<VAR> vartable, list<INS *> &backlist, compiler::front::ast::FunctionDefine *func) {
        //处理函数参数
        int argsize = func->args->args.size();
        for (int i = 0; i < argsize; i++) {
            auto arg = func->args->args[i]->name;
            if (arg->nodetype == IdentifierType) {
                vartable.push_back(VAR(arg->name, 0, i - 2 - argsize));//value 0表示为数字
            } else if (arg->nodetype == ArrayIdentifierType) {
                vartable.push_back(VAR(arg->name, -1, i - 2 - argsize));//value -1表示为指针
            }
        }
        //处理函数体
        for (auto item = func->body->blockItem.begin(); item != func->body->blockItem.end(); item++) {
            switch ((*item)->nodetype) {
                case IfStatementType: {
                    int id = ifStatCount++;
                    backlist.push_back(new Lable("if_con_" + to_string(id)));
                    generateExpression(vartable, backlist, static_cast<IfStatement *>((*item))->cond);//计算条件到r2
                    backlist.push_back(new CMPBEQ("r2", "#0", "if_else_" + to_string(id)));
                    backlist.push_back(new Lable("if_true_" + to_string(id)));
                    //iftrue体
                    if (static_cast<IfStatement *>(*item)->trueBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(*item)->trueBlock));
                    } else {
                        generateStmt(vartable, backlist, static_cast<IfStatement *>(*item)->trueBlock);
                    }
                    backlist.push_back(new B("if_end_" + to_string(id)));
                    backlist.push_back(new Lable("if_else_" + to_string(id)));
                    //ifelse体
                    if (static_cast<IfStatement *>(*item)->elseBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(*item)->elseBlock));
                    } else if (static_cast<IfStatement *>(*item)->elseBlock->nodetype != VoidStatementType) {
                        generateStmt(vartable, backlist, static_cast<IfStatement *>(*item)->elseBlock);
                    }
                    backlist.push_back(new Lable("if_end_" + to_string(id)));
                    break;
                }
                case FunctionCallType: {
                    generateFuncCall(vartable, backlist, static_cast<FunctionCall *>((*item)));
                    break;
                }
                case DeclareStatementType: {
                    for (auto subNode:static_cast<DeclareStatement *>(*item)->declareList) {
                        switch (subNode->nodetype) {
                            case ConstArrayType:
                            case ArrayDeclareType:
                            case ArrayDeclareWithInitType: {
                                generateBackArray(vartable, backlist, subNode);
                                break;
                            }
                            case VarDeclareWithInitType: {
                                //把右值放到r2
                                generateExpression(vartable, backlist, static_cast<VarDeclareWithInit *>(subNode)->value);
                                string name = subNode->name->name;
                                vartable.push_back(VAR(name, 0, tableIndex++));
                                //存到内存中
                                backlist.push_back(new STR(2));
                                break;
                            }
                            case VarDeclareType: {
                                string name = subNode->name->name;
                                vartable.push_back(VAR(name, 0, tableIndex++));
                                backlist.push_back(new STR(0));
                                break;
                            }
                        }
                    }
                    break;
                }
                case AssignStmtType: {
                    if (static_cast<AssignStmt *>(*item)->name->nodetype == ArrayIdentifierType) {
                        getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(static_cast<AssignStmt *>(*item)->name));
                        generateExpression(vartable, backlist, static_cast<AssignStmt *>(*item)->rightExpr);
                        backlist.push_back(new STR(2, address("r6", 0)));
                        break;
                    } else {
                        string name = static_cast<AssignStmt *>(*item)->name->name;
                        generateExpression(vartable, backlist, static_cast<AssignStmt *>(*item)->rightExpr);
                        //判断全局变量还是局部变量
                        int index = tableFind(vartable, name);
                        if (index == -1) {
                            //全局变量
                            //取地址到r3
                            backlist.push_back(new MOV32(3, name));
                            backlist.push_back(new STR(2, address("r3", 0)));
                        } else {
                            backlist.push_back(new STR(2, address("fp", -8 - 4 * vartable[index].index)));
                        }
                        break;
                    }
                }
                case ReturnStatementType: {
                    generateExpression(vartable, backlist, static_cast<ReturnStatement *>((*item))->returnExp);
                    backlist.push_back(new MOV(0, 2, "reg2reg"));
                    break;
                }
            }
        }
    }

    void generateExpression(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Expression *expression) {
        switch (expression->nodetype) {
            case FunctionCallType: {
                generateFuncCall(vartable, backlist, static_cast<FunctionCall *>(expression));
                backlist.push_back(new MOV(2, 0, "reg2reg"));
                break;
            }
            case NumberExpressionType: {
                //右值为数字
                int value = 0;
                value = static_cast<NumberExpression *>(expression)->value;
                if (value < 65535) {
                    backlist.push_back(new MOV(2, value));
                } else {
                    backlist.push_back(new MOV32(2, value));
                }
                break;
            }
            case ConstArrayType:
            case ArrayDeclareType:
            case ArrayIdentifierType: {
                string name = static_cast<ArrayIdentifier *>(expression)->name;
                int index = tableFind(vartable, name);
                if (index == -1) {
                    int offset = 0;
                    int allNum = 1;
                    for (int i = 1; i < globalVartable[name]->arrayIndex.size(); i++) {
                        if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype == NumberExpressionType) {
                            offset += globalVartable[name]->arrayIndex[i] *
                                      static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index[i - 1])->value;
                        } else {
                            allNum = 0;
                        }
                    }
                    if (static_cast<ArrayIdentifier *>(expression)->index.back()->nodetype != NumberExpressionType) {
                        allNum = 0;
                    }
                    //处理最后一个下标
                    if (static_cast<ArrayIdentifier *>(expression)->index.back()->nodetype == NumberExpressionType) {
                        if (allNum)
                            offset += static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value;
                        else
                            backlist.push_back(new MOV(5, static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value * 4));
                    } else {
                        generateExpression(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index.back());
                        backlist.push_back(new OP("lsl", 2, 2, "#2"));
                        backlist.push_back(new MOV(5, 2));
                    }
                    if (allNum) {
                        //取地址到r4
                        backlist.push_back(new MOV32(4, name));
                        backlist.push_back(new LDR(2, address("r4", -4 * offset)));
                    } else {
                        for (int i = 1; i < globalVartable[name]->arrayIndex.size(); i++) {
                            if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype != NumberExpressionType) {
                                generateExpression(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index[i - 1]);
                                backlist.push_back(new MOV(3, globalVartable[name]->arrayIndex[i] * 4));
                                backlist.push_back(new MLA("r5", "r2", "r3", "r5"));
                            }
                        }
                        if (offset * 4 <= 65535) {
                            backlist.push_back(new OP("add", 5, 5, "#" + to_string(offset * 4)));
                        } else {
                            backlist.push_back(new MOV32(4, offset * 4));
                            backlist.push_back(new OP("add", 5, 5, 4));
                        }
                        backlist.push_back(new MOV32(4, name));
                        backlist.push_back(new LDR(2, address("r4", "-r5")));
                    }
                } else {
                    int offset = 0;
                    int allNum = 1;
                    for (int i = 1; i < vartable[index].arrayIndex.size(); i++) {
                        if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype == NumberExpressionType) {
                            offset += vartable[index].arrayIndex[i] *
                                      static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index[i - 1])->value;
                        } else {
                            allNum = 0;
                        }
                    }
                    if (static_cast<ArrayIdentifier *>(expression)->index.back()->nodetype != NumberExpressionType) {
                        allNum = 0;
                    }
                    //处理最后一个下标
                    if (static_cast<ArrayIdentifier *>(expression)->index.back()->nodetype == NumberExpressionType) {
                        if (allNum)
                            offset += static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value;
                        else
                            backlist.push_back(new MOV(5, static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value * 4));
                    } else {
                        generateExpression(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index.back());
                        backlist.push_back(new OP("lsl", 2, 2, "#2"));
                        backlist.push_back(new MOV(5, 2));
                    }
                    if (allNum) {
                        backlist.push_back(new LDR(2, address("fp", -8 - 4 * (vartable[index].index + offset))));
                    } else {
                        for (int i = 1; i < vartable[index].arrayIndex.size(); i++) {
                            if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype != NumberExpressionType) {
                                generateExpression(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index[i - 1]);
                                backlist.push_back(new MOV(3, vartable[index].arrayIndex[i] * 4));
                                backlist.push_back(new MLA("r5", "r2", "r3", "r5"));
                            }
                        }
                        if ((2 + offset + vartable[index].index) * 4 <= 65535) {
                            backlist.push_back(new OP("add", 5, 5, "#" + to_string((2 + offset + vartable[index].index) * 4)));
                        } else {
                            backlist.push_back(new MOV32(4, (2 + offset + vartable[index].index) * 4));
                            backlist.push_back(new OP("add", 5, 5, 4));
                        }
                        backlist.push_back(new LDR(2, address("fp", "-r5")));
                    }
                }
                break;
            }
            case IdentifierType: {
                //右值为单个字母
                string name = static_cast<Identifier *>(expression)->name;
                int index = tableFind(vartable, name);
                if (index == -1) {
                    //全局变量
                    //取地址到r4
                    backlist.push_back(new MOV32(4, name));
                    //读到r2
                    backlist.push_back(new LDR(2, address("r4", 0)));
                } else {
                    //读到r2
                    backlist.push_back(new LDR(2, address("fp", -8 - 4 * vartable[index].index)));
                }
                break;
            }
            case UnaryExpressionType: {
                //UnaryExpressionType，计算后将计算后的值放在r2
                generateUnaryExpression(vartable, backlist, static_cast<UnaryExpression *>(expression), 2);
                break;
            }
            case BinaryExpressionType: {
                //右值为BinaryExpression，计算后将计算后的值放在r2
                generateBinaryExpression(vartable, backlist, static_cast<BinaryExpression *>(expression), 2);
                break;
            }
        }
    }

    void generateBinaryExpression(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::BinaryExpression *expression, int pos) {
        int reg1;
        int reg2;//最终结果
        int reg3;
        if (pos == 1) {
            reg1 = 1;
            reg2 = 1;
            reg3 = 2;
        }
        if (pos == 2) {
            reg1 = 1;
            reg2 = 2;
            reg3 = 3;
        }
        if (pos == 3) {
            reg1 = 2;
            reg2 = 3;
            reg3 = 3;
        }
        //处理左侧
        switch (expression->leftExpr->nodetype) {
            case NumberExpressionType: {
                int value = static_cast<NumberExpression *>(expression->leftExpr)->value;
                //TODO 小于0的情况没有考虑
                if (value < 65535) {
                    backlist.push_back(new MOV(reg1, value));
                } else {
                    backlist.push_back(new MOV32(reg1, value));
                }
                break;
            }
            case ArrayIdentifierType: {
                getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(expression->leftExpr));
                backlist.push_back(new LDR(reg1, address("r6", 0)));
                break;
            }
            case IdentifierType: {
                //右值为单个字母
                string name = static_cast<Identifier *>(expression->leftExpr)->name;
                int index = tableFind(vartable, name);
                if (index == -1) {
                    //全局变量
                    //取地址到r4
                    backlist.push_back(new MOV32(4, name));
                    //读到r2
                    backlist.push_back(new LDR(reg1, address("r4", 0)));
                } else {
                    backlist.push_back(new LDR(reg1, address("fp", -8 - 4 * vartable[index].index)));
                }
                break;
            }
            case BinaryExpressionType: {
                generateBinaryExpression(vartable, backlist, static_cast<BinaryExpression *>(expression->leftExpr), reg1);
                break;
            }
            case UnaryExpressionType: {
                generateUnaryExpression(vartable, backlist, static_cast<UnaryExpression *>(expression->rightExpr), reg1);
                break;
            }
        }
        //处理右侧
        switch (expression->rightExpr->nodetype) {
            case NumberExpressionType: {
                int value = static_cast<NumberExpression *>(expression->rightExpr)->value;
                //TODO 小于0的情况没有考虑
                if (value < 65535) {
                    backlist.push_back(new MOV(reg3, value));
                } else {
                    backlist.push_back(new MOV32(reg3, value));
                }
                break;
            }
            case ArrayIdentifierType: {
                getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(expression->rightExpr));
                backlist.push_back(new LDR(reg3, address("r6", 0)));
                break;
            }
            case IdentifierType: {
                //右值为单个字母
                string name = static_cast<Identifier *>(expression->rightExpr)->name;
                int index = tableFind(vartable, name);
                if (index == -1) {
                    //全局变量
                    //取地址到r4
                    backlist.push_back(new MOV32(4, name));
                    //读到r3
                    backlist.push_back(new LDR(reg3, address("r4", 0)));
                } else {
                    backlist.push_back(new LDR(reg3, address("fp", -8 - 4 * vartable[index].index)));
                }
                break;
            }
            case BinaryExpressionType: {
                generateBinaryExpression(vartable, backlist, static_cast<BinaryExpression *>(expression->rightExpr), reg3);
                break;
            }
            case UnaryExpressionType: {
                generateUnaryExpression(vartable, backlist, static_cast<UnaryExpression *>(expression->rightExpr), reg3);
                break;
            }
        }
        switch (expression->op) {
            case ADD: {
                backlist.push_back(new OP("add", reg2, reg1, reg3));
                break;
            }
            case SUB: {
                backlist.push_back(new OP("sub", reg2, reg1, reg3));
                break;
            }
            case MUL: {
                backlist.push_back(new OP("mul", reg2, reg1, reg3));
                break;
            }
            case DIV: {
                backlist.push_back(new OP("sdiv", reg2, reg1, reg3));
                break;
            }
            /*case MOD: {
                backlist.push_back(new OP("aaa", reg2, reg1, reg3));
                break;
            }*/
            case EQ: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("eq", reg2, 1));
                backlist.push_back(new MOV("ne", reg2, 0));
                break;
            }
            case NE: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("eq", reg2, 0));
                backlist.push_back(new MOV("ne", reg2, 1));
                break;
            }
            case LT: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("lt", reg2, 1));
                backlist.push_back(new MOV("ge", reg2, 0));
                break;
            }
            case LE: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("le", reg2, 1));
                backlist.push_back(new MOV("gt", reg2, 0));
                break;
            }
            case GT: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("le", reg2, 0));
                backlist.push_back(new MOV("gt", reg2, 1));
                break;
            }
            case GE: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("lt", reg2, 0));
                backlist.push_back(new MOV("ge", reg2, 1));
                break;
            }
            case AND_OP: {
                backlist.push_back(new OP("and", reg2, reg1, reg3));
                break;
            }
            case OR_OP: {
                backlist.push_back(new OP("orr", reg2, reg1, reg3));
                break;
            }
            case NOT_EQUAL: {
                backlist.push_back(new CMP(reg1, reg3));
                backlist.push_back(new MOV("eq", reg2, 0));
                backlist.push_back(new MOV("ne", reg2, 1));
                break;
            }
        }
    }

    void generateUnaryExpression(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::UnaryExpression *expression, int pos) {
        int reg1;
        int reg2;//最终结果
        if (pos == 2) {
            reg1 = 1;
            reg2 = 2;
        }
        if (pos == 1) {
            reg1 = 2;
            reg2 = 1;
        }
        if (pos == 3) {
            reg1 = 2;
            reg2 = 3;
        }
        switch (expression->right->nodetype) {
            case NumberExpressionType: {
                int value = static_cast<NumberExpression *>(expression->right)->value;
                //TODO 小于0的情况没有考虑
                if (value < 65535) {
                    backlist.push_back(new MOV(reg2, value));
                } else {
                    backlist.push_back(new MOV32(reg2, value));
                }
                break;
            }
            case ArrayIdentifierType: {
                getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(expression->right));
                backlist.push_back(new LDR(reg2, address("r6", 0)));
                break;
            }
            case IdentifierType: {
                //右值为单个字母
                string name = static_cast<Identifier *>(expression->right)->name;
                int index = tableFind(vartable, name);
                if (index == -1) {
                    //全局变量
                    //取地址到r4
                    backlist.push_back(new MOV32(4, name));
                    //读到r3
                    backlist.push_back(new LDR(reg2, address("r4", 0)));
                } else {
                    backlist.push_back(new LDR(reg2, address("fp", -8 - 4 * vartable[index].index)));
                }
                break;
            }
            case BinaryExpressionType: {
                generateBinaryExpression(vartable, backlist, static_cast<BinaryExpression *>(expression->right), reg2);
                break;
            }
            case UnaryExpressionType: {
                generateUnaryExpression(vartable, backlist, static_cast<UnaryExpression *>(expression->right), reg2);
                break;
            }
        }
        switch (expression->op) {
            case SUB: {
                backlist.push_back(new MOV(reg1, 0));
                backlist.push_back(new OP("sub", reg2, reg1, reg2));
                break;
            }
            case NOT_EQUAL: {
                backlist.push_back(new CMP(reg2, "#0"));
                backlist.push_back(new MOV("eq", reg2, 1));
                backlist.push_back(new MOV("ne", reg2, 0));
                break;
            }
        }
    }

    void generateBackArray(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Declare *array) {
        switch (array->nodetype) {
            case ConstArrayType:
            case ArrayDeclareType: {
                string name = array->name->name;
                VAR temp = VAR(name, 0, tableIndex);
                for (auto index:static_cast<ArrayIdentifier *>(array->name)->index) {
                    temp.arrayIndex.push_back(static_cast<NumberExpression *>(index)->value);
                }
                for (auto value:static_cast<ArrayDeclare *>(array)->initVal->initValList) {
                    generateExpression(vartable, backlist, value);
                    //存到内存中
                    backlist.push_back(new STR(2));
                    tableIndex++;
                }
                vartable.push_back(temp);
                break;
            }
            case ArrayDeclareWithInitType: {
                string name = array->name->name;
                VAR temp = VAR(name, 0, tableIndex);
                for (auto index:static_cast<ArrayIdentifier *>(array->name)->index) {
                    temp.arrayIndex.push_back(static_cast<NumberExpression *>(index)->value);
                }
                for (auto value:static_cast<ArrayDeclareWithInit *>(array)->initVal->initValList) {
                    generateExpression(vartable, backlist, value);
                    //存到内存中
                    backlist.push_back(new STR(2));
                    tableIndex++;
                }
                vartable.push_back(temp);
                break;
            }
        }
    }

    //获得地址到r6
    void getArrayIdentAddress(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::ArrayIdentifier *arrayIdentifier) {
        string name = arrayIdentifier->name;
        int index = tableFind(vartable, name);
        if (index == -1) {
            int offset = 0;
            int allNum = 1;
            for (int i = 1; i < globalVartable[name]->arrayIndex.size(); i++) {
                if (arrayIdentifier->index[i - 1]->nodetype == NumberExpressionType) {
                    offset += globalVartable[name]->arrayIndex[i] *
                              static_cast<NumberExpression *>(arrayIdentifier->index[i - 1])->value;
                } else {
                    allNum = 0;
                }
            }
            if (arrayIdentifier->index.back()->nodetype != NumberExpressionType) {
                allNum = 0;
            }
            //处理最后一个下标
            if (arrayIdentifier->index.back()->nodetype == NumberExpressionType) {
                if (allNum)
                    offset += static_cast<NumberExpression *>(arrayIdentifier->index.back())->value;
                else
                    backlist.push_back(new MOV(5, static_cast<NumberExpression *>(arrayIdentifier->index.back())->value * 4));
            } else {
                generateExpression(vartable, backlist, arrayIdentifier->index.back());
                backlist.push_back(new OP("lsl", 2, 2, "#2"));
                backlist.push_back(new MOV(5, 2));
            }
            if (allNum) {
                //取地址到r4
                backlist.push_back(new MOV32(4, name));
                //获得地址
                //backlist.push_back(new LDR(2,address("r4", - 4 * offset)));
                backlist.push_back(new OP("sub", 6, 4, "#" + to_string(4 * offset)));
            } else {
                for (int i = 1; i < globalVartable[name]->arrayIndex.size(); i++) {
                    if (arrayIdentifier->index[i - 1]->nodetype != NumberExpressionType) {
                        generateExpression(vartable, backlist, arrayIdentifier->index[i - 1]);
                        backlist.push_back(new MOV(3, globalVartable[name]->arrayIndex[i] * 4));
                        backlist.push_back(new MLA("r5", "r2", "r3", "r5"));
                    }
                }
                if (offset * 4 <= 65535) {
                    backlist.push_back(new OP("add", 5, 5, "#" + to_string(offset * 4)));
                } else {
                    backlist.push_back(new MOV32(4, offset * 4));
                    backlist.push_back(new OP("add", 5, 5, 4));
                }
                backlist.push_back(new MOV32(4, name));
                //backlist.push_back(new LDR(2,address("r4","-r5")));
                //获得地址
                backlist.push_back(new OP("sub", 6, 4, 5));
            }
        } else {
            int offset = 0;
            int allNum = 1;
            for (int i = 1; i < vartable[index].arrayIndex.size(); i++) {
                if (arrayIdentifier->index[i - 1]->nodetype == NumberExpressionType) {
                    offset += vartable[index].arrayIndex[i] *
                              static_cast<NumberExpression *>(arrayIdentifier->index[i - 1])->value;
                } else {
                    allNum = 0;
                }
            }
            if (arrayIdentifier->index.back()->nodetype != NumberExpressionType) {
                allNum = 0;
            }
            //处理最后一个下标
            if (arrayIdentifier->index.back()->nodetype == NumberExpressionType) {
                if (allNum)
                    offset += static_cast<NumberExpression *>(arrayIdentifier->index.back())->value;
                else
                    backlist.push_back(new MOV(5, static_cast<NumberExpression *>(arrayIdentifier->index.back())->value * 4));
            } else {
                generateExpression(vartable, backlist, arrayIdentifier->index.back());
                backlist.push_back(new OP("lsl", 2, 2, "#2"));
                backlist.push_back(new MOV(5, 2));
            }
            if (allNum) {
                //获得地址
                //backlist.push_back(new LDR(2,address("fp", -8 - 4 * (vartable[index].index + offset))));
                backlist.push_back(new OP("sub", 6, "fp", "#" + to_string(8 + 4 * (vartable[index].index + offset))));
            } else {
                for (int i = 1; i < vartable[index].arrayIndex.size(); i++) {
                    if (arrayIdentifier->index[i - 1]->nodetype != NumberExpressionType) {
                        generateExpression(vartable, backlist, arrayIdentifier->index[i - 1]);
                        backlist.push_back(new MOV(3, vartable[index].arrayIndex[i] * 4));
                        backlist.push_back(new MLA("r5", "r2", "r3", "r5"));
                    }
                }
                if ((2 + offset + vartable[index].index) * 4 <= 65535) {
                    backlist.push_back(new OP("add", 5, 5, "#" + to_string((2 + offset + vartable[index].index) * 4)));
                } else {
                    backlist.push_back(new MOV32(4, (2 + offset + vartable[index].index) * 4));
                    backlist.push_back(new OP("add", 5, 5, 4));
                }
                //backlist.push_back(new LDR(2,address("fp","-r5")));
                //获得地址
                backlist.push_back(new OP("sub", 6, "fp", "r5"));
            }
        }
    }

    //处理函数调用
    void generateFuncCall(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::FunctionCall *functionCall) {
        //处理参数
        for (auto arg: functionCall->args->args) {
            if (arg->nodetype == IdentifierType) {
                string name = static_cast<Identifier *>(arg)->name;
                int index = tableFind(vartable, name);
                if (index != -1) {
                    if (vartable[index].arrayIndex.size() > 0) {
                        //数组
                        backlist.push_back(new OP("sub", 4, "fp", "#" + to_string(8 + vartable[index].index * 4)));
                        backlist.push_back(new STR(4));
                    } else {
                        //数字
                        backlist.push_back(new LDR(2, address("fp", -8 - 4 * (vartable[index].index))));
                        backlist.push_back(new STR(2));
                    }
                } else {
                    if (globalVartable[name]->arrayIndex.size() > 0) {
                        backlist.push_back(new MOV32(4, name));
                        backlist.push_back(new STR(4));
                    } else {
                        backlist.push_back(new MOV32(4, name));
                        backlist.push_back(new LDR(2, address("r4", 0)));
                        backlist.push_back(new STR(2));
                    }
                }
            } else {
                generateExpression(vartable, backlist, arg);
                backlist.push_back(new STR(2));
            }
        }
        //处理寄存器
        //backlist.push_back(new STMDB("func"));
        //跳转
        backlist.push_back(new BL(functionCall->name->name));
        //回复现场
        //backlist.push_back(new LDMIA("func"));
    }

    //处理if_while的block
    void generateBlock(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Block *block) {
        for (auto item = block->blockItem.begin(); item != block->blockItem.end(); item++) {
            switch ((*item)->nodetype) {
                case IfStatementType: {
                    int id = ifStatCount++;
                    backlist.push_back(new Lable("if_con_" + to_string(id)));
                    generateExpression(vartable, backlist, static_cast<IfStatement *>((*item))->cond);//计算条件到r2
                    backlist.push_back(new CMPBEQ("r2", "#0", "if_else_" + to_string(id)));
                    backlist.push_back(new Lable("if_true_" + to_string(id)));
                    //iftrue体
                    if (static_cast<IfStatement *>(*item)->trueBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(*item)->trueBlock));
                    } else {
                        generateStmt(vartable, backlist, static_cast<IfStatement *>(*item)->trueBlock);
                    }
                    backlist.push_back(new B("if_end_" + to_string(id)));
                    backlist.push_back(new Lable("if_else_" + to_string(id)));
                    //ifelse体
                    if (static_cast<IfStatement *>(*item)->elseBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(*item)->elseBlock));
                    } else if (static_cast<IfStatement *>(*item)->elseBlock->nodetype != VoidStatementType) {
                        generateStmt(vartable, backlist, static_cast<IfStatement *>(*item)->elseBlock);
                    }
                    backlist.push_back(new Lable("if_end_" + to_string(id)));
                    break;
                }
                case FunctionCallType: {
                    generateFuncCall(vartable, backlist, static_cast<FunctionCall *>((*item)));
                    break;
                }
                case DeclareStatementType: {
                    for (auto subNode:static_cast<DeclareStatement *>(*item)->declareList) {
                        switch (subNode->nodetype) {
                            case ConstArrayType:
                            case ArrayDeclareType:
                            case ArrayDeclareWithInitType: {
                                generateBackArray(vartable, backlist, subNode);
                                break;
                            }
                            case VarDeclareWithInitType: {
                                //把右值放到r2
                                generateExpression(vartable, backlist, static_cast<VarDeclareWithInit *>(subNode)->value);
                                string name = subNode->name->name;
                                vartable.push_back(VAR(name, 0, tableIndex++));
                                //存到内存中
                                backlist.push_back(new STR(2));
                                break;
                            }
                            case VarDeclareType: {
                                string name = subNode->name->name;
                                vartable.push_back(VAR(name, 0, tableIndex++));
                                backlist.push_back(new STR(0));
                                break;
                            }
                        }
                    }
                    break;
                }
                case AssignStmtType: {
                    if (static_cast<AssignStmt *>(*item)->name->nodetype == ArrayIdentifierType) {
                        getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(static_cast<AssignStmt *>(*item)->name));
                        generateExpression(vartable, backlist, static_cast<AssignStmt *>(*item)->rightExpr);
                        backlist.push_back(new STR(2, address("r6", 0)));
                        break;
                    } else {
                        string name = static_cast<AssignStmt *>(*item)->name->name;
                        generateExpression(vartable, backlist, static_cast<AssignStmt *>(*item)->rightExpr);
                        //判断全局变量还是局部变量
                        int index = tableFind(vartable, name);
                        if (index == -1) {
                            //全局变量
                            //取地址到r3
                            backlist.push_back(new MOV32(3, name));
                            backlist.push_back(new STR(2, address("r3", 0)));
                        } else {
                            backlist.push_back(new STR(2, address("fp", -8 - 4 * vartable[index].index)));
                        }
                        break;
                    }
                }
                case ReturnStatementType: {
                    generateExpression(vartable, backlist, static_cast<ReturnStatement *>((*item))->returnExp);
                    backlist.push_back(new MOV(0, 2, "reg2reg"));
                    break;
                }
            }
        }
    }

    //处理if_while的单条语句
    void generateStmt(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Node *stmt) {
        switch ((stmt)->nodetype) {
            case IfStatementType: {
                int id = ifStatCount++;
                backlist.push_back(new Lable("if_con_" + to_string(id)));
                generateExpression(vartable, backlist, static_cast<IfStatement *>((stmt))->cond);//计算条件到r2
                backlist.push_back(new CMPBEQ("r2", "#0", "if_else_" + to_string(id)));
                backlist.push_back(new Lable("if_true_" + to_string(id)));
                //iftrue体
                if (static_cast<IfStatement *>(stmt)->trueBlock->nodetype == BlockType) {
                    generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(stmt)->trueBlock));
                } else {
                    generateStmt(vartable, backlist, static_cast<IfStatement *>(stmt)->trueBlock);
                }
                backlist.push_back(new B("if_end_" + to_string(id)));
                backlist.push_back(new Lable("if_else_" + to_string(id)));
                //ifelse体
                if (static_cast<IfStatement *>(stmt)->elseBlock->nodetype == BlockType) {
                    generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(stmt)->elseBlock));
                } else if (static_cast<IfStatement *>(stmt)->elseBlock->nodetype != VoidStatementType) {
                    generateStmt(vartable, backlist, static_cast<IfStatement *>(stmt)->elseBlock);
                }
                backlist.push_back(new Lable("if_end_" + to_string(id)));
                break;
            }
            case FunctionCallType: {
                generateFuncCall(vartable, backlist, static_cast<FunctionCall *>((stmt)));
                break;
            }
            case DeclareStatementType: {
                for (auto subNode:static_cast<DeclareStatement *>(stmt)->declareList) {
                    switch (subNode->nodetype) {
                        case ConstArrayType:
                        case ArrayDeclareType:
                        case ArrayDeclareWithInitType: {
                            generateBackArray(vartable, backlist, subNode);
                            break;
                        }
                        case VarDeclareWithInitType: {
                            //把右值放到r2
                            generateExpression(vartable, backlist, static_cast<VarDeclareWithInit *>(subNode)->value);
                            string name = subNode->name->name;
                            vartable.push_back(VAR(name, 0, tableIndex++));
                            //存到内存中
                            backlist.push_back(new STR(2));
                            break;
                        }
                        case VarDeclareType: {
                            string name = subNode->name->name;
                            vartable.push_back(VAR(name, 0, tableIndex++));
                            backlist.push_back(new STR(0));
                            break;
                        }
                    }
                }
                break;
            }
            case AssignStmtType: {
                if (static_cast<AssignStmt *>(stmt)->name->nodetype == ArrayIdentifierType) {
                    getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(static_cast<AssignStmt *>(stmt)->name));
                    generateExpression(vartable, backlist, static_cast<AssignStmt *>(stmt)->rightExpr);
                    backlist.push_back(new STR(2, address("r6", 0)));
                    break;
                } else {
                    string name = static_cast<AssignStmt *>(stmt)->name->name;
                    generateExpression(vartable, backlist, static_cast<AssignStmt *>(stmt)->rightExpr);
                    //判断全局变量还是局部变量
                    int index = tableFind(vartable, name);
                    if (index == -1) {
                        //全局变量
                        //取地址到r3
                        backlist.push_back(new MOV32(3, name));
                        backlist.push_back(new STR(2, address("r3", 0)));
                    } else {
                        backlist.push_back(new STR(2, address("fp", -8 - 4 * vartable[index].index)));
                    }
                    break;
                }
            }
            case ReturnStatementType: {
                generateExpression(vartable, backlist, static_cast<ReturnStatement *>((stmt))->returnExp);
                backlist.push_back(new MOV(0, 2, "reg2reg"));
                break;
            }
        }
    }
}

