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
  typedef std::function<void(bool, const tradingpb::Asset*, TimeStamp)>
      FuncOnCtrl;

 public:
  CtrlConditionHelper() {}
  virtual ~CtrlConditionHelper() {}

 public:
  virtual bool isValid(const tradingpb::CtrlCondition& cc) = 0;

  virtual void procCtrl(const tradingpb::CtrlCondition& cc,
                        FuncOnCtrl onctrl) = 0;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITION__H__