#ifndef __TRADINGCORE2_CTRLCONDITION_MONTHDAY_H__
#define __TRADINGCORE2_CTRLCONDITION_MONTHDAY_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class CCMonthDay final : public CtrlConditionHelper {
 public:
  static void regCtrlConditionHelper();

 protected:
  CCMonthDay() {}
  virtual ~CCMonthDay() {}

 public:
  virtual void getIndicators(std::set<std::string>& indicators,
                             const tradingpb::CtrlCondition& cc) override {}

  virtual void* newCtrlConditionData() override { return NULL; }

  virtual void deleteCtrlConditionData(void* pData) override {}

  virtual void clearCtrlConditionData(void* pData) override {}

  virtual bool isValid(const tradingpb::CtrlCondition& cc,
                       CtrlType ct) override;

  virtual bool canCtrl(const Exchange& exchange, const Wallet& wallet,
                       const IndicatorMap& mapIndicators,
                       const tradingpb::CtrlCondition& cc, bool issim,
                       CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                       Assets& assets, void* pData) override;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITION_MONTHDAY_H__