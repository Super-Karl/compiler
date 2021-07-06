//
// Created by wnx on 2021/6/13.
//

#include "ir.h"
#include <assert.h>

namespace compiler::mid::ir {

  bool OperatorName::operator==(OperatorName &other) {
    switch (other.getType()) {
      case Type::Var:
        return this->name == other.name;
      case Type::Imm:
        return this->value == other.value;
      case Type::Void:
        assert(this->getType() == Type::Void);
        return true;
    };
  }
}