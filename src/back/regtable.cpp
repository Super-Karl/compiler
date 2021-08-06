//
// Created by karl on 2021/8/2.
//

#include "regtable.h"

namespace compiler::back {

    std::vector<int> regtable(10);

    std::stack<std::vector<int>> regtableStack;

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
        for(int j=1; j<m;j++)
        {
            std::cout<<" "<<regtable[j];
        }
        std::cout<<"]\n";
    }

    //为计算表达式清空寄存器
    void clearRegTableForCalExp() {
        for (int i = 1; i < m; i++) {
            regtable[i] = 0;
        }
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
        std::cout<<"寄存器栈满";
        //return -1;
        return 0;
    }

    //占用i寄存器
    bool useRegForCalExp(int i) {
        if (regtable[i] == 0) {
            regtable[i] = 1;
            return true;
        } else {
            std::cout << "寄存器使用错误";
            return false;
        }
    }

    void freeRegForCalExp(int i) {
        if (1 <= i && i < m) {
            regtable[i] = 0;
        }
    }
}
