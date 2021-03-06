#ifndef __TRADINGCORE2_CTRLCONDITION_WEEKDAYEX_H__
#define __TRADINGCORE2_CTRLCONDITION_WEEKDAYEX_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class CCWeekDayEx final : public CtrlConditionHelper {
 public:
  static void regCtrlConditionHelper();

 protected:
  struct _Data {
    time_t lastTs;
    time_t preTs;

    _Data() : lastTs(0), preTs(0) {}

    void clear() { this->lastTs = this->preTs; }
  };

 protected:
  CCWeekDayEx() {}
  virtual ~CCWeekDayEx() {}

 public:
  virtual void getIndicators(std::set<std::string>& indicators,
                             const tradingpb::CtrlCondition& cc) override {}

  virtual void* newCtrlConditionData() override { return new _Data(); }

  virtual void deleteCtrlConditionData(void* pData) override {
    assert(pData != NULL);
    auto pMyData = static_cast<_Data*>(pData);
    delete pMyData;
  }

  virtual void clearCtrlConditionData(void* pData) override {
    assert(pData != NULL);
    auto pMyData = static_cast<_Data*>(pData);
    pMyData->clear();
  }

  virtual bool isValid(const tradingpb::CtrlCondition& cc,
                       CtrlType ct) override;

  virtual bool canCtrl(const Exchange& exchange, const Wallet& wallet,
                       const IndicatorMap& mapIndicators,
                       const tradingpb::CtrlCondition& cc, bool issim,
                       CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                       Assets& assets, void* pData) override;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITION_WEEKDAYEX_H__