//
// Created by wnx on 2021/8/11.
//

#include "deadCode.h"

namespace compiler::mid::pass {

  void pass::deadCodeElimination(AssignIR *assign) {
  }

  void pass::deadCodeElimination(FunDefIR *funDef) {
  }

  void pass::deadCodeElimination(FunCallIR *funCall) {
  }

  void pass::deadCodeElimination(GlobalData *globalData) {
  }

  void pass::deadCodeElimination(LabelIR *label) {
  }

  void pass::deadCodeElimination(JmpIR *jump) {
  }

  void pass::deadCodeElimination(RetIR *ret) {
  }

  void pass::deadCodeElimination(AllocaIR *alloca) {
  }

  void pass::deadCodeElimination(LoadIR *load) {
  }
  void pass::deadCodeElimination(StoreIR *store) {
  }
  void pass::deadCodeElimination(IRList &ir) {
    for (auto i : ir)
      deadCodeElimination(i);
  }
  void deadCodeElimination(IR *ir) {

  }

};// namespace compiler::mid::pass