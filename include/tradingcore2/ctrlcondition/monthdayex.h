#ifndef __TRADINGCORE2_CTRLCONDITION_MONTHDAYEX_H__
#define __TRADINGCORE2_CTRLCONDITION_MONTHDAYEX_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class CCMonthDayEx final : public CtrlConditionHelper {
 public:
  static void regCtrlConditionHelper();

 public:
  struct _Data {
    time_t lastTs;

    _Data() : lastTs(0) {}
  };

 protected:
  CCMonthDayEx() {}
  virtual ~CCMonthDayEx() {}

 public:
  virtual void* newCtrlConditionData() override { return new _Data(); }

  virtual void deleteCtrlConditionData(void* pData) override {
    assert(pData != NULL);
    auto pMyData = static_cast<_Data*>(pData);
    delete pMyData;
  }

  virtual bool isValid(const tradingpb::CtrlCondition& cc,
                       CtrlType ct) override;

  virtual void procCtrl(const tradingpb::CtrlCondition& cc, bool issim,
                        CtrlType ct, TimeStamp ts, int index, void* pData,
                        FuncOnCtrl onctrl) override;
};

CR2END

#endif  // __TRADINGCORE2_CTRLCONDITION_MONTHDAYEX_H__