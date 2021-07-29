//
// Created by karl on 2021/7/29.
//

#ifndef COMPILER_BACK_H
#define COMPILER_BACK_H

#include <enum/enums.h>

namespace compiler::back {
    class INS {

    };

    class Lable : public INS {
    public :
        LableType lableType;
        int count;

        Lable(LableType lableType, int count);

        virtual ~Lable();
    };

    class Globle : public INS {
    public:

    };
}
#endif //COMPILER_BACK_H
