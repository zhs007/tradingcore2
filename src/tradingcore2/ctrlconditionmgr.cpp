#include <math.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/ctrlconditionmgr.h>

#include <functional>

CR2BEGIN

CtrlConditionMgr* CtrlConditionMgr::getSingleton() {
  // for c++ 11
  static CtrlConditionMgr s_mgr;

  return &s_mgr;
}

void CtrlConditionMgr::regCtrlCondition(const char* name,
                                        CtrlConditionHelper* cc) {
  PairCtrlCondition p(name, cc);

  auto ret = this->m_mapCtrlCondition.insert(p);
  assert(ret.second);
}

CtrlConditionHelper* CtrlConditionMgr::getCtrlCondition(const char* name) {
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second;
  }

  return NULL;
}

void CtrlConditionMgr::release() {
  for (auto it = this->m_mapCtrlCondition.begin();
       it != this->m_mapCtrlCondition.end(); ++it) {
    delete it->second;
  }

  this->m_mapCtrlCondition.clear();
}

CR2END