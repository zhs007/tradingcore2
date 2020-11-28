#ifndef __TRADINGCORE2_CTRLCONDITION__H__
#define __TRADINGCORE2_CTRLCONDITION__H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class CtrlConditionHelper {
 public:
  typedef std::function<void(bool, CtrlType, TimeStamp)> FuncOnCtrl;

 public:
  CtrlConditionHelper() {}
  virtual ~CtrlConditionHelper() {}

 public:
  virtual void* newCtrlConditionData() = 0;

  virtual void deleteCtrlConditionData(void* pData) = 0;

  virtual bool isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) = 0;

  virtual void procCtrl(const tradingpb::CtrlCondition& cc, bool issim,
                        CtrlType ct, TimeStamp ts, int index, void* pData,
                        FuncOnCtrl onctrl) = 0;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITION__H__