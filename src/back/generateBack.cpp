//
// Created by karl on 2021/7/29.
//
//TODO 改while和if的编号 处理除法 数组传递参数 运行时库 处理模运算
#include "generateBack.h"
#include "regtable.h"

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

    int whileCount = 0;

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
                generateBackFunction(vartable, backlist, func);

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
        generateBlock(vartable, backlist, func->body, -1);
    }

    void generateBackArray(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Declare *array) {
        switch (array->nodetype) {
            case ArrayDeclareWithInitType:
            case ConstArrayType:
            case ArrayDeclareType: {
                string name = array->name->name;
                VAR temp = VAR(name, 0, tableIndex);
                for (auto index:static_cast<ArrayIdentifier *>(array->name)->index) {
                    temp.arrayIndex.push_back(static_cast<NumberExpression *>(index)->value);
                }
                for (auto value:static_cast<ArrayDeclare *>(array)->initVal->initValList) {
                    int reg = generateExp(vartable, backlist, value);
                    //存到内存中
                    backlist.push_back(new STR(reg));
                    freeRegForCalExp(reg);
                    tableIndex++;
                }
                vartable.push_back(temp);
                break;
            }
        }
    }

    //获得地址到r6
    int getArrayIdentAddress(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::ArrayIdentifier *expression) {
        string name = expression->name;
        int index = tableFind(vartable, name);
        int regtomul;
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
                else {
                    regtomul = getCanUseRegForCalExp();
                    backlist.push_back(new LDR(regtomul, static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value * 4));
                }
            } else {
                regtomul = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index.back());
                backlist.push_back(new OP("lsl", regtomul, regtomul, "#2"));
            }
            if (allNum) {
                //取地址到regyomul
                regtomul = getCanUseRegForCalExp();
                backlist.push_back(new LDR(regtomul, name));
                int reg = getCanUseRegForCalExp();
                backlist.push_back(new LDR(reg, 4 * offset));
                backlist.push_back(new OP("add", regtomul, regtomul, reg));
                freeRegForCalExp(reg);
                //backlist.push_back(new LDR(regtomul, address(regtomul, -4 * offset)));
            } else {
                for (int i = 1; i < globalVartable[name]->arrayIndex.size(); i++) {
                    if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype != NumberExpressionType) {
                        int reg1 = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index[i - 1]);
                        int reg2 = getCanUseRegForCalExp();
                        backlist.push_back(new LDR(2, globalVartable[name]->arrayIndex[i] * 4));
                        backlist.push_back(new MLA(regtomul, reg1, reg2, regtomul));
                        freeRegForCalExp(reg1);
                        freeRegForCalExp(reg2);
                    }
                }
                if (offset * 4 <= 65535) {
                    backlist.push_back(new OP("add", regtomul, regtomul, "#" + to_string(offset * 4)));
                } else {
                    int reg1 = getCanUseRegForCalExp();
                    backlist.push_back(new LDR(reg1, offset * 4));
                    backlist.push_back(new OP("add", regtomul, regtomul, reg1));
                    freeRegForCalExp(reg1);
                }
                int reg1 = getCanUseRegForCalExp();
                backlist.push_back(new LDR(reg1, name));
                backlist.push_back(new OP("add", regtomul, reg1, regtomul));
                //backlist.push_back(new LDR(regtomul, address("r" + to_string(reg1), "-r" + to_string(regtomul))));
                freeRegForCalExp(reg1);
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
                else {
                    regtomul = getCanUseRegForCalExp();
                    backlist.push_back(new LDR(regtomul, static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value * 4));
                }
            } else {
                regtomul = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index.back());
                backlist.push_back(new OP("lsl", regtomul, regtomul, "#2"));
            }
            if (allNum) {
                regtomul = getCanUseRegForCalExp();
                int reg = getCanUseRegForCalExp();
                backlist.push_back(new LDR(reg, 8 + 4 * (vartable[index].index + offset)));
                backlist.push_back(new OP("sub", regtomul, 11, reg));
                freeRegForCalExp(reg);
                //backlist.push_back(new LDR(regtomul, address("fp", -8 - 4 * (vartable[index].index + offset))));
            } else {
                for (int i = 1; i < vartable[index].arrayIndex.size(); i++) {
                    if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype != NumberExpressionType) {
                        int reg1 = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index[i - 1]);
                        int reg2 = getCanUseRegForCalExp();
                        backlist.push_back(new MOV(reg2, vartable[index].arrayIndex[i] * 4));
                        backlist.push_back(new MLA(regtomul, reg1, reg2, regtomul));
                        freeRegForCalExp(reg1);
                        freeRegForCalExp(reg2);
                    }
                }
                if ((2 + offset + vartable[index].index) * 4 <= 65535) {
                    backlist.push_back(new OP("add", regtomul, regtomul, "#" + to_string((2 + offset + vartable[index].index) * 4)));
                } else {
                    int reg1 = getCanUseRegForCalExp();
                    backlist.push_back(new LDR(reg1, (2 + offset + vartable[index].index) * 4));
                    backlist.push_back(new OP("add", regtomul, regtomul, reg1));
                    freeRegForCalExp(reg1);
                }
                int reg1 = getCanUseRegForCalExp();
                backlist.push_back(new OP("sub", regtomul, 11, regtomul));
                //backlist.push_back(new LDR(regtomul, address("fp", "-r" + to_string(regtomul))));
                freeRegForCalExp(reg1);
            }
        }
        return regtomul;
    }

    //处理函数调用
    void generateFuncCall(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::FunctionCall *functionCall) {
        //处理参数
        for (auto arg: functionCall->args->args) {
            int reg = generateExp(vartable, backlist, arg);
            if (functionCall->name->name == "putint" || functionCall->name->name == "putch") {
                backlist.push_back(new MOV(0, reg, "reg2reg"));
            } else {
                backlist.push_back(new STR(reg));
            }
            freeRegForCalExp(reg);
        }
        //跳转
        backlist.push_back(new BL(functionCall->name->name));
        //清除临时数据,维护堆栈
        backlist.push_back(new OP("add", "sp", "sp", "#" + to_string(functionCall->args->args.size() * 4)));
    }

    //处理block
    void generateBlock(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Block *block, int nowWhileId) {
        for (auto item = block->blockItem.begin(); item != block->blockItem.end(); item++) {
            switch ((*item)->nodetype) {
                case BreakStatemetType: {
                    backlist.push_back(new B("while_end_" + to_string(nowWhileId)));
                    break;
                }
                case ContinueStatementType: {
                    backlist.push_back(new B("while_con_" + to_string(nowWhileId)));
                    break;
                }
                case WhileStatementType: {
                    int id = whileCount++;
                    backlist.push_back(new Lable("while_con_" + to_string(id)));
                    //计算条件
                    int reg = generateExp(vartable, backlist, static_cast<WhileStatement *>((*item))->cond);
                    backlist.push_back(new CMPBEQ("r" + to_string(reg), "#0", "while_end_" + to_string(id)));
                    freeRegForCalExp(reg);
                    backlist.push_back(new Lable("while_body_" + to_string(id)));
                    //while体
                    if (static_cast<WhileStatement *>(*item)->loopBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<WhileStatement *>(*item)->loopBlock), id);
                    } else {
                        generateStmt(vartable, backlist, static_cast<WhileStatement *>(*item)->loopBlock, id);
                    }
                    backlist.push_back(new B("while_con_" + to_string(id)));
                    backlist.push_back(new Lable("while_end_" + to_string(id)));
                    break;
                }
                case IfStatementType: {
                    int id = ifStatCount++;
                    backlist.push_back(new Lable("if_con_" + to_string(id)));
                    //计算条件
                    int reg = generateExp(vartable, backlist, static_cast<IfStatement *>((*item))->cond);
                    backlist.push_back(new CMPBEQ("r" + to_string(reg), "#0", "if_else_" + to_string(id)));
                    freeRegForCalExp(reg);
                    backlist.push_back(new Lable("if_true_" + to_string(id)));
                    //iftrue体
                    if (static_cast<IfStatement *>(*item)->trueBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(*item)->trueBlock), nowWhileId);
                    } else {
                        generateStmt(vartable, backlist, static_cast<IfStatement *>(*item)->trueBlock, nowWhileId);
                    }
                    backlist.push_back(new B("if_end_" + to_string(id)));
                    backlist.push_back(new Lable("if_else_" + to_string(id)));
                    //ifelse体
                    if (static_cast<IfStatement *>(*item)->elseBlock->nodetype == BlockType) {
                        generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(*item)->elseBlock), nowWhileId);
                    } else if (static_cast<IfStatement *>(*item)->elseBlock->nodetype != VoidStatementType) {
                        generateStmt(vartable, backlist, static_cast<IfStatement *>(*item)->elseBlock, nowWhileId);
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
                                int reg = generateExp(vartable, backlist, static_cast<VarDeclareWithInit *>(subNode)->value);
                                string name = subNode->name->name;
                                vartable.push_back(VAR(name, 0, tableIndex++));
                                //存到内存中
                                backlist.push_back(new STR(reg));
                                freeRegForCalExp(reg);
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
                        int reg1 = getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(static_cast<AssignStmt *>(*item)->name));
                        int reg2 = generateExp(vartable, backlist, static_cast<AssignStmt *>(*item)->rightExpr);
                        backlist.push_back(new STR(reg2, address(reg1)));
                        freeRegForCalExp(reg1);
                        freeRegForCalExp(reg2);
                        break;
                    } else {
                        string name = static_cast<AssignStmt *>(*item)->name->name;
                        int reg = generateExp(vartable, backlist, static_cast<AssignStmt *>(*item)->rightExpr);
                        //判断全局变量还是局部变量
                        int index = tableFind(vartable, name);
                        if (index == -1) {
                            //全局变量
                            //取地址到r3
                            int reg1 = getCanUseRegForCalExp();
                            backlist.push_back(new LDR(reg1, name));
                            backlist.push_back(new STR(reg, address(reg1)));
                            freeRegForCalExp(reg1);
                        } else {
                            backlist.push_back(new STR(reg, address("fp", -8 - 4 * vartable[index].index)));
                        }
                        freeRegForCalExp(reg);
                        break;
                    }
                }
                case ReturnStatementType: {
                    int reg = generateExp(vartable, backlist, static_cast<ReturnStatement *>((*item))->returnExp);
                    if (reg != 0) {
                        backlist.push_back(new MOV(0, reg, "reg2reg"));
                        freeRegForCalExp(reg);
                    }
                    backlist.push_back(new LDMIA());
                    break;
                }
            }
        }
    }

    //处理if_while的单条语句
    void generateStmt(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Node *stmt, int nowWhileId) {
        switch ((stmt)->nodetype) {
            case BreakStatemetType: {
                backlist.push_back(new B("while_end_" + to_string(nowWhileId)));
                break;
            }
            case ContinueStatementType: {
                backlist.push_back(new B("while_con_" + to_string(nowWhileId)));
                break;
            }
            case WhileStatementType: {
                int id = whileCount++;
                backlist.push_back(new Lable("while_con_" + to_string(id)));
                //计算条件
                int reg = generateExp(vartable, backlist, static_cast<WhileStatement *>((stmt))->cond);
                backlist.push_back(new CMPBEQ("r" + to_string(reg), "#0", "while_end_" + to_string(id)));
                freeRegForCalExp(reg);
                backlist.push_back(new Lable("while_body_" + to_string(id)));
                //while体
                if (static_cast<WhileStatement *>(stmt)->loopBlock->nodetype == BlockType) {
                    generateBlock(vartable, backlist, static_cast<Block *>(static_cast<WhileStatement *>(stmt)->loopBlock), id);
                } else {
                    generateStmt(vartable, backlist, static_cast<WhileStatement *>(stmt)->loopBlock, id);
                }
                backlist.push_back(new B("while_con_" + to_string(id)));
                backlist.push_back(new Lable("while_end_" + to_string(id)));
                break;
            }
            case IfStatementType: {
                int id = ifStatCount++;
                backlist.push_back(new Lable("if_con_" + to_string(id)));
                //计算条件
                int reg = generateExp(vartable, backlist, static_cast<IfStatement *>((stmt))->cond);
                backlist.push_back(new CMPBEQ("r" + to_string(reg), "#0", "if_else_" + to_string(id)));
                freeRegForCalExp(reg);
                backlist.push_back(new Lable("if_true_" + to_string(id)));
                //iftrue体
                if (static_cast<IfStatement *>(stmt)->trueBlock->nodetype == BlockType) {
                    generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(stmt)->trueBlock), nowWhileId);
                } else {
                    generateStmt(vartable, backlist, static_cast<IfStatement *>(stmt)->trueBlock, nowWhileId);
                }
                backlist.push_back(new B("if_end_" + to_string(id)));
                backlist.push_back(new Lable("if_else_" + to_string(id)));
                //ifelse体
                if (static_cast<IfStatement *>(stmt)->elseBlock->nodetype == BlockType) {
                    generateBlock(vartable, backlist, static_cast<Block *>(static_cast<IfStatement *>(stmt)->elseBlock), nowWhileId);
                } else if (static_cast<IfStatement *>(stmt)->elseBlock->nodetype != VoidStatementType) {
                    generateStmt(vartable, backlist, static_cast<IfStatement *>(stmt)->elseBlock, nowWhileId);
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
                            int reg = generateExp(vartable, backlist, static_cast<VarDeclareWithInit *>(subNode)->value);
                            string name = subNode->name->name;
                            vartable.push_back(VAR(name, 0, tableIndex++));
                            //存到内存中
                            backlist.push_back(new STR(reg));
                            freeRegForCalExp(reg);
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
                    int reg1 = getArrayIdentAddress(vartable, backlist, static_cast<ArrayIdentifier *>(static_cast<AssignStmt *>(stmt)->name));
                    int reg2 = generateExp(vartable, backlist, static_cast<AssignStmt *>(stmt)->rightExpr);
                    backlist.push_back(new STR(reg2, address(reg1)));
                    freeRegForCalExp(reg1);
                    freeRegForCalExp(reg2);
                    break;
                } else {
                    string name = static_cast<AssignStmt *>(stmt)->name->name;
                    int reg = generateExp(vartable, backlist, static_cast<AssignStmt *>(stmt)->rightExpr);
                    //判断全局变量还是局部变量
                    int index = tableFind(vartable, name);
                    if (index == -1) {
                        //全局变量
                        //取地址到r3
                        int reg1 = getCanUseRegForCalExp();
                        backlist.push_back(new LDR(reg1, name));
                        backlist.push_back(new STR(reg, address(reg1)));
                        freeRegForCalExp(reg1);
                    } else {
                        backlist.push_back(new STR(reg, address("fp", -8 - 4 * vartable[index].index)));
                    }
                    freeRegForCalExp(reg);
                    break;
                }
            }
            case ReturnStatementType: {
                int reg = generateExp(vartable, backlist, static_cast<ReturnStatement *>((stmt))->returnExp);
                if (reg != 0) {
                    backlist.push_back(new MOV(0, reg, "reg2reg"));
                    freeRegForCalExp(reg);
                }
                backlist.push_back(new LDMIA());
                break;
            }
        }
    }

    //计算函数，返回正数或0为结果存放的寄存器值，负数为栈地址相对fp的偏移量，初次调用，记录sp的内容放到r12中，清空寄存器，9,10寄存器保留给使用内存时运算
    //新计算函数
    int generateExp(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::Expression *expression) {
        switch (expression->nodetype) {
            case NumberExpressionType : {
                int reg = getCanUseRegForCalExp();
                backlist.push_back(new LDR(reg, static_cast<NumberExpression *>(expression)->value));
                return reg;
            }
            case FunctionCallType: {
                backlist.push_back(new PUSH("r1-r8"));
                pushregtable();
                generateFuncCall(vartable, backlist, static_cast<FunctionCall *>(expression));
                popregtable();
                backlist.push_back(new POP("r1-r8"));
                int reg = getCanUseRegForCalExp();
                backlist.push_back(new MOV(reg, 0, "reg2reg"));
                return reg;
            }
            case IdentifierType: {
                //右值为单个字母
                int reg = getCanUseRegForCalExp();
                string name = static_cast<Identifier *>(expression)->name;
                int index = tableFind(vartable, name);
                if (index == -1) {
                    //全局变量
                    //取地址到ldr reg,=name
                    backlist.push_back(new LDR(reg, name));
                    //读到ldr reg,[reg]
                    backlist.push_back(new LDR(reg, address(reg)));
                } else {
                    //读到ldr reg, [fp, #offset]
                    backlist.push_back(new LDR(reg, address("fp", -8 - 4 * vartable[index].index)));
                }
                return reg;
            }
            case ArrayIdentifierType: {
                string name = static_cast<ArrayIdentifier *>(expression)->name;
                int index = tableFind(vartable, name);
                int regtomul;
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
                        else {
                            regtomul = getCanUseRegForCalExp();
                            backlist.push_back(new LDR(regtomul, static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value * 4));
                        }
                    } else {
                        regtomul = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index.back());
                        backlist.push_back(new OP("lsl", regtomul, regtomul, "#2"));
                    }
                    if (allNum) {
                        //取地址到regyomul
                        regtomul = getCanUseRegForCalExp();
                        backlist.push_back(new LDR(regtomul, name));
                        backlist.push_back(new LDR(regtomul, address(regtomul, 4 * offset)));
                    } else {
                        for (int i = 1; i < globalVartable[name]->arrayIndex.size(); i++) {
                            if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype != NumberExpressionType) {
                                int reg1 = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index[i - 1]);
                                int reg2 = getCanUseRegForCalExp();
                                backlist.push_back(new LDR(2, globalVartable[name]->arrayIndex[i] * 4));
                                backlist.push_back(new MLA(regtomul, reg1, reg2, regtomul));
                                freeRegForCalExp(reg1);
                                freeRegForCalExp(reg2);
                            }
                        }
                        if (offset * 4 <= 65535) {
                            backlist.push_back(new OP("add", regtomul, regtomul, "#" + to_string(offset * 4)));
                        } else {
                            int reg1 = getCanUseRegForCalExp();
                            backlist.push_back(new LDR(reg1, offset * 4));
                            backlist.push_back(new OP("add", regtomul, regtomul, reg1));
                            freeRegForCalExp(reg1);
                        }
                        int reg1 = getCanUseRegForCalExp();
                        backlist.push_back(new LDR(reg1, name));
                        backlist.push_back(new LDR(regtomul, address("r" + to_string(reg1), "r" + to_string(regtomul))));
                        freeRegForCalExp(reg1);
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
                        else {
                            regtomul = getCanUseRegForCalExp();
                            backlist.push_back(new LDR(regtomul, static_cast<NumberExpression *>(static_cast<ArrayIdentifier *>(expression)->index.back())->value * 4));
                        }
                    } else {
                        regtomul = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index.back());
                        backlist.push_back(new OP("lsl", regtomul, regtomul, "#2"));
                    }
                    if (allNum) {
                        regtomul = getCanUseRegForCalExp();
                        backlist.push_back(new LDR(regtomul, address("fp", -8 - 4 * (vartable[index].index + offset))));
                    } else {
                        for (int i = 1; i < vartable[index].arrayIndex.size(); i++) {
                            if (static_cast<ArrayIdentifier *>(expression)->index[i - 1]->nodetype != NumberExpressionType) {
                                int reg1 = generateExp(vartable, backlist, static_cast<ArrayIdentifier *>(expression)->index[i - 1]);
                                int reg2 = getCanUseRegForCalExp();
                                backlist.push_back(new MOV(reg2, vartable[index].arrayIndex[i] * 4));
                                backlist.push_back(new MLA(regtomul, reg1, reg2, regtomul));
                                freeRegForCalExp(reg1);
                                freeRegForCalExp(reg2);
                            }
                        }
                        if ((2 + offset + vartable[index].index) * 4 <= 65535) {
                            backlist.push_back(new OP("add", regtomul, regtomul, "#" + to_string((2 + offset + vartable[index].index) * 4)));
                        } else {
                            int reg1 = getCanUseRegForCalExp();
                            backlist.push_back(new LDR(reg1, (2 + offset + vartable[index].index) * 4));
                            backlist.push_back(new OP("add", regtomul, regtomul, reg1));
                            freeRegForCalExp(reg1);
                        }
                        backlist.push_back(new LDR(regtomul, address("fp", "-r" + to_string(regtomul))));
                    }
                }
                return regtomul;
            }
            case BinaryExpressionType: {
                int reg = generateBinaryExp(vartable, backlist, static_cast<BinaryExpression *>(expression));
                return reg;
            }
            case UnaryExpressionType: {
                int reg = generateUnaryExp(vartable, backlist, static_cast<UnaryExpression *>(expression));
                return reg;
            }
            default: {
                cout << "生成表达式错误" << expression->nodetype << endl;
            }
        }
        return -1;
    }

    int generateBinaryExp(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::BinaryExpression *expression) {
        int reg1 = generateExp(vartable, backlist, expression->leftExpr);
        int reg2 = generateExp(vartable, backlist, expression->rightExpr);
        switch (expression->op) {
            //TODO mod的运算
            case ADD: {
                backlist.push_back(new OP("add", reg1, reg1, reg2));
                break;
            }
            case SUB: {
                backlist.push_back(new OP("sub", reg1, reg1, reg2));
                break;
            }
            case MUL: {
                backlist.push_back(new OP("mul", reg1, reg1, reg2));
                break;
            }
            case DIV: {
                backlist.push_back(new OP("sdiv", reg1, reg1, reg2));
                break;
            }
            case EQ: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("eq", reg1, 1));
                backlist.push_back(new MOV("ne", reg1, 0));
                break;
            }
            case NE: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("eq", reg1, 0));
                backlist.push_back(new MOV("ne", reg1, 1));
                break;
            }
            case LT: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("lt", reg1, 1));
                backlist.push_back(new MOV("ge", reg1, 0));
                break;
            }
            case LE: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("le", reg1, 1));
                backlist.push_back(new MOV("gt", reg1, 0));
                break;
            }
            case GT: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("le", reg1, 0));
                backlist.push_back(new MOV("gt", reg1, 1));
                break;
            }
            case GE: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("lt", reg1, 0));
                backlist.push_back(new MOV("ge", reg1, 1));
                break;
            }
            case AND_OP: {
                backlist.push_back(new OP("and", reg1, reg1, reg2));
                break;
            }
            case OR_OP: {
                backlist.push_back(new OP("orr", reg1, reg1, reg2));
                break;
            }
            case NOT_EQUAL: {
                backlist.push_back(new CMP(reg1, reg2));
                backlist.push_back(new MOV("eq", reg1, 0));
                backlist.push_back(new MOV("ne", reg1, 1));
                break;
            }
            default: {
                cout << "未匹配运算符" << endl;
            }
        }
        freeRegForCalExp(reg2);
        return reg1;
    }

    int generateUnaryExp(vector<VAR> &vartable, list<INS *> &backlist, compiler::front::ast::UnaryExpression *expression) {
        int reg = generateExp(vartable, backlist, expression->right);
        switch (expression->op) {
            case SUB: {
                int regnum = getCanUseRegForCalExp();
                backlist.push_back(new MOV(regnum, 0));
                backlist.push_back(new OP("sub", reg, regnum, reg));
                freeRegForCalExp(regnum);
                break;
            }
            case NOT_EQUAL: {
                backlist.push_back(new CMP(reg, "#0"));
                backlist.push_back(new MOV("eq", reg, 1));
                backlist.push_back(new MOV("ne", reg, 0));
                break;
            }
        }
        return reg;
    }
}

