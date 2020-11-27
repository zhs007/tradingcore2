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