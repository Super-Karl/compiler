//
// Created by karl on 2021/7/29.
//

#include "back.h"

compiler::back::Lable::~Lable() = default;

compiler::back::Lable::Lable(LableType lableType, int count) : lableType(lableType), count(count) {}
