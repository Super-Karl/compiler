//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <assert.h>
#include <iostream>
#include <stack>
#include <vector>

namespace compiler::mid::ir {
  unsigned int RecordTable::id = 0;
  VarInfo *RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return varTable[name];//
    else if (father != nullptr)
      return father->searchVar(name);
    throw std::out_of_range("no var name " + name);
  }

  void RecordTable::insertVar(std::string name, VarInfo *v) {
    this->bundle.addVar(name,v->getUseName(),BlockLabel(),std::vector<IR*>::iterator(),this);
    varTable.insert({name, v});
  }

  bool RecordTable::canExprAssign(std::string op1, std::string op2, std::vector<int> index1, std::vector<int> index2) {
    auto var1 = this->searchVar(op1);
    auto var2 = this->searchVar(op2);
    return var1->canAssign(index1) && var2->canAssign(index2);
  }

  /*
   *VarInfo
   */

  VarInfo::VarInfo(std::string name, std::vector<int> &inShape, std::vector<int> &inValue, bool canAssign, bool isConst) {

    this->arrayName = name;
    this->isConst = isConst;
    this->isArray = canAssign;
    this->shape = inShape;
    varUse.resize(inValue.size());
    for (auto i = 0; i < inValue.size(); i++) {
      auto var = VarRedefChain("", inValue[i], true);
      varUse[i].push_front(var);
    }
  }

  VarInfo::VarInfo(std::string name, int value, bool canAssign, bool isConst)
      : isConst(isConst), isArray(false), arrayName("") {
    this->varUse.resize(1);
    this->varUse[0].emplace_front(VarRedefChain(name, value, canAssign));
 b  }

  VarInfo::VarInfo(std::string name, std::vector<int> &inValue, std::initializer_list<int> inShape, bool isConst) {
    this->arrayName = std::move(name);
    this->shape = inShape;
    this->isConst = isConst;
    this->isArray = true;
    varUse.resize(inValue.size());
    for (int i = 0; i < inShape.size(); ++i) {
      auto var = VarRedefChain("", inValue[i], true);
      varUse[i].push_front(var);
    }
  }


  int VarInfo::getArrayVal(std::vector<int> index) {
    int tmp = 1;
    int sum = 0;
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= shape[i];
    }
    if (sum < varUse.size())
      return varUse[sum].front().val;
    throw std::runtime_error("out of index");
  }

  int VarInfo::getArrayIndex(std::vector<int> index) {
//    assert(this->shape.size() == index.size());
    int tmp = 1;
    int sum = 0;
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= shape[i];
    }
    if (sum < varUse.size())
      return sum;
    throw std::runtime_error("out of index");
  }

  int VarInfo::getVal() {
    return this->varUse[0].front().val;
  }

  std::string VarInfo::getUseName() {
    return this->varUse[0].front().defName;
  }

  bool VarInfo::canAssign(std::vector<int> index) {
    if (isArray)
      return varUse[this->getArrayIndex(index)].front().canAssign;
    else
      return varUse[0].front().canAssign;
  }

  void VarInfo::addVarUse(VarRedefChain var, std::vector<int> index) {
    if (isArray) {
      this->varUse[getArrayIndex(std::move(index))].push_front(var);
    } else {
      this->varUse[0].push_front(var);
    }
  }

  ElemType RecordTable::getFunRet(std::string funcName) {
    auto tmp = funDecl.find(funcName);
    if (tmp != funDecl.end())
      return funDecl[funcName];
    else if (this->father != nullptr)
      return father->getFunRet(funcName);
  }

  void RecordTable::setFunRet(std::pair<std::string, ElemType> pair) {
    funDecl.insert(pair);
  }
}// namespace compiler::mid::ir

namespace compiler::mid::ir {
  std::stack<std::pair<LabelIR *, LabelIR *>> RecordTable::labelPairs;
  void RecordTable::pushLabelPair(LabelIR *label1, LabelIR *label2) {
    labelPairs.push(std::make_pair(label1, label2));
  }
  void RecordTable::popLabelPair() {
    labelPairs.pop();
  }
  std::pair<LabelIR *, LabelIR *> &RecordTable::getTopLabel() {
    return labelPairs.top();
  }
}// namespace compiler::mid::ir

namespace compiler::mid::ir{
  int Stream::id = 0;
  void Stream::addUse(std::string name,BlockLabel label,std::vector<IR*>::iterator it){
    Definition def;
    def.blockLabel = label;
    def.position = it;
    def.name = name;
  }
  Stream::Stream() {
    this->uid = id++;
  }
  Definition& Stream::getTop() {
    return defChain.back();
  }
  void Bundle::addUse(std::string name ,BlockLabel blockLabel,std::vector<IR*>::iterator it,RecordTable *record){
    try {
      VarInfo* varInfo = record->searchVar(name);
      int uid = varInfo->uid;
      Stream &stream = this->varDefs[uid];
      stream.addUse(name,blockLabel,it);
    }catch (...) {
      throw std::runtime_error("Can not find var");
    }
  }
  void Bundle::addVar(std::string defName,std::string name, BlockLabel blockLabel,std::vector<IR*>::iterator it,RecordTable *record){
    Stream stream;
    int uid = stream.uid;
    stream.defName = defName;
    stream.addUse(name,blockLabel,it);
    this->varDefs.insert(std::make_pair(uid,stream));
  }
  void Bundle::addStream(Stream &s) {
    this->varDefs.insert(std::make_pair(s.uid,s));
  }
  std::map<int,Stream>::iterator Bundle::find(int key){
    return this->varDefs.find(key);
  }
  std::map<int,Stream>::iterator Bundle::end(){
    return this->varDefs.end();
  }
  std::map<int,Stream>::iterator Bundle::begin(){
    return this->varDefs.begin();
  }

  Bundle mergeRecord(RecordTable *record,IRList &list1,Bundle &bundle1,IRList &list2,Bundle &bundle2){
    Bundle bundle;
    for (auto &item: bundle1){
      int uid = item.first;
      Stream stream1 = item.second;
      auto it = bundle2.find(uid);
      if (it == bundle2.end()){
        continue;
      }

      Stream &stream2 = it->second;
      if (stream1.getTop()!=stream2.getTop()){
        auto phi = new AssignIR(OperatorName("%"+std::to_string(record->getID())),stream1.getTop().name,OperatorCode::PhiMov);
        list1.push_back(phi);
        list2.push_back(phi);
        stream1.addUse(phi->dest.name,BlockLabel(),std::vector<IR*>::iterator());
      }
      bundle.addStream(stream1);
    }
    return bundle;
  }
  void mergeRecordSlave(RecordTable *record,IRList &list1,Bundle bundle1,IRList &list2,Bundle bundle2){
    for (auto &item: bundle1){
      int uid = item.first;
      Stream stream1 = item.second;
      auto it = bundle2.find(uid);
      if (it == bundle2.end()){
        continue;
      }
      Stream &stream2 = it->second;
      if (stream1.getTop()!=stream2.getTop()){
        auto phi = new AssignIR(OperatorName("%"+std::to_string(record->getID())),stream1.getTop().name,OperatorCode::PhiMov);
        //list1.push_back(phi);
        list2.push_back(phi);
      }
    }
  }
  Bundle mergeRecord(std::vector<Bundle> l,RecordTable* record){
    Bundle Merged;
    Bundle &bundle1 = l.front();
    auto it = l.begin();

    for (auto &item:bundle1){
      bool flag = false;
      int uid = item.first;
      for (++it;it!=l.end();++it){
        auto tmpIt = it->find(uid);
        if (tmpIt != it->end()){
          if (tmpIt->second.getTop() != item.second.getTop()){
            flag = true;
            break;
          }
        }
      }
      if (flag){
        OperatorName dest("%"+std::to_string(record->getID()));
        //for ()
      }
    }
  }
}//namespace compiler::mid::ir for Bundle