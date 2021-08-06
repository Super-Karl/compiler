//
// Created by karl on 2021/8/2.
//

#ifndef COMPILER_REGTABLE_H
#define COMPILER_REGTABLE_H
#define m 11

#include<stack>
#include<vector>
#include<list>
#include<iostream>

namespace compiler::back {

    void pushregtable();

    void popregtable();

    void frintRegtable();

    void clearRegTableForCalExp();

    int getCanUseRegForCalExp();

    bool useRegForCalExp(int i);

    void freeRegForCalExp(int i);
}
#endif //COMPILER_REGTABLE_H
