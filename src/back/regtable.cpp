//
// Created by karl on 2021/8/2.
//

#include "regtable.h"
#include "back.h"

namespace compiler::back {

    std::vector<int> regtable(m);

    std::stack<std::vector<int>> regtableStack;

    extern list<INS *> backlist;

    int reg8used = 0;

    int reg9used = 0;

    int reg10used = 0;

    int regStackInMemDeepth = 0;

    //推入寄存器表
    void pushregtable(){
        regtableStack.push(regtable);
    }

    //弹出
    void popregtable(){
        regtable = regtableStack.top();
        regtableStack.pop();
    }

    void frintRegtable()
    {
        std::cout<<";["<<regtable[0];
        for(int j=1; j<30;j++)
        {
            std::cout<<" "<<regtable[j];
        }
        std::cout<<"]\n";
    }

    //返回可用的寄存器，满了则返回-1,然后分配内存
    int getCanUseRegForCalExp() {
        for (int i = 1; i < m; i++) {
            if (regtable[i] == 0) {
                regtable[i] = 1;

                frintRegtable();
                return i;
            }
        }
    }

    void freeRegForCalExp(int i) {
        if (1 <= i && i < m) {
            regtable[i] = 0;
        }
    }
}
