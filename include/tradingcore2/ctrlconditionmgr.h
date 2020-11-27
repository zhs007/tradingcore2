#ifndef __TRADINGCORE2_CTRLCONDITIONMGR__H__
#define __TRADINGCORE2_CTRLCONDITIONMGR__H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class CtrlConditionHelper;

class CtrlConditionMgr {
 public:
  typedef std::pair<std::string, CtrlConditionHelper*> PairCtrlCondition;
  typedef std::map<std::string, CtrlConditionHelper*> MapCtrlCondition;

 public:
  static CtrlConditionMgr* getSingleton();

 public:
  void regCtrlCondition(const char* name, CtrlConditionHelper* cc);

  CtrlConditionHelper* getCtrlCondition(const char* name);

 public:
  // isValid - 检查 cc 协议的完整性，返回 0 表示有效，返回 1 表示没有支持这个
  // cc，返回 -1 表示错误
  int isValid(const tradingpb::CtrlCondition& cc, CtrlType ct);
  // isValidStrategy - 检查 strategy 协议的完整性，返回 0 表示有效，返回 1
  // 表示没有有未支持的 CtrlCondition，返回 -1 表示有错误
  int isValidStrategy(const tradingpb::Strategy& strategy);

 protected:
  CtrlConditionMgr() {}
  ~CtrlConditionMgr() { this->release(); }

 protected:
  void release();

 protected:
  MapCtrlCondition m_mapCtrlCondition;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITIONMGR__H__