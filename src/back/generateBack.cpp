//
// Created by karl on 2021/7/29.
//

#include "back.h"
#include "generateBack.h"
#include <list>
#include <fstream>
#include <front/ast/AstNode.h>
#include <unordered_map>

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
                generateBackFunction(backlist,func);
                backlist.push_back(new LDMIA());
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
                                int value = 0;
                                string name = subNode->name->name;
                                if (static_cast<VarDeclareWithInit *>(subNode)->value->nodetype == NumberExpressionType) {
                                    value = static_cast<NumberExpression *>(static_cast<VarDeclareWithInit *>(subNode)->value)->value;
                                }
                                int index = tableIndex[nowfunc];
                                vartable[nowfunc][name] = new VAR(name, value, index);
                                tableIndex[nowfunc]++;
                                if (value < 65535) {
                                    backlist.push_back(new MOVE(index, value));
                                }
                                break;
                            }
                            case VarDeclareType:{
                                string name = subNode->name->name;
                                int index = tableIndex[nowfunc];
                                vartable[nowfunc][name] = new VAR(name, 0, index);
                                tableIndex[nowfunc]++;
                                break;
                            }
                        }
                    }
                    break;
                }
                case AssignStmtType:{
                    int value = 0;
                    string name = static_cast<AssignStmt*>(*item)->name->name;
                    if (static_cast<AssignStmt*>(*item)->rightExpr->nodetype == NumberExpressionType) {
                        value = static_cast<NumberExpression *>(static_cast<AssignStmt*>(*item)->rightExpr)->value;
                    }
                    vartable[nowfunc][name]->value=value;
                    int index = vartable[nowfunc][name]->index;
                    if (value < 65535) {
                        backlist.push_back(new MOVE(index, value));
                    }
                    break;
                }
                case ReturnStatementType:{
                    switch(static_cast<ReturnStatement*>((*item))->returnExp->nodetype)
                    {
                        case IdentifierType:
                        {
                            string name = static_cast<Identifier*>(static_cast<ReturnStatement *>((*item))->returnExp)->name;
                            backlist.push_back(new MOVE(0,vartable[nowfunc][name]->index,"reg2reg"));
                            break;
                        }
                        case NumberExpressionType:
                        {
                            int value = static_cast<NumberExpression*>(static_cast<ReturnStatement *>((*item))->returnExp)->value;
                            backlist.push_back(new MOVE(0,value));
                        }
                    }
                    break;
                }
            }
        }
    }
}

