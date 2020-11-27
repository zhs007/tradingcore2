#ifndef __TRADINGCORE2_CTRLCONDITION_BUYANDHOLD_H__
#define __TRADINGCORE2_CTRLCONDITION_BUYANDHOLD_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class CCBuyAndHold final : public CtrlConditionHelper {
 public:
  static void regCtrlConditionHelper();

 protected:
  CCBuyAndHold() {}
  virtual ~CCBuyAndHold() {}

 public:
  virtual bool isValid(const tradingpb::CtrlCondition& cc,
                       CtrlType ct) override;

  virtual void procCtrl(const tradingpb::CtrlCondition& cc, bool issim,
                        CtrlType ct, TimeStamp ts, int index,
                        FuncOnCtrl onctrl) override;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITION_BUYANDHOLD_H__