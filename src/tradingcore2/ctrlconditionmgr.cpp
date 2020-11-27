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

// isValid - 检查 cc 协议的完整性，返回 0 表示有效，返回 1 表示没有支持这个
// cc，返回 -1 表示错误
int CtrlConditionMgr::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  auto name = cc.indicator();
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second->isValid(cc, ct) ? 0 : -1;
  }

  return 1;
}

// isValidStrategy - 检查 strategy 协议的完整性，返回 0 表示有效，返回 1
// 表示没有有未支持的 CtrlCondition，返回 -1 表示有错误
int CtrlConditionMgr::isValidStrategy(const tradingpb::Strategy& strategy) {
  bool hasnocc = false;
  for (auto i = 0; i < strategy.buy_size(); i++) {
    auto cc = strategy.buy(i);
    auto ret = this->isValid(cc, CT_BUY);
    if (ret == -1) {
      return -1;
    } else if (ret == 1) {
      hasnocc = true;
    }
  }

  if (hasnocc) {
    return 1;
  }

  return 0;
}

CR2END