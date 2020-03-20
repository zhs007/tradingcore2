#ifndef __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__
#define __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class SingleIndicator : public Strategy {
 public:
  SingleIndicator(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange), m_pIndicator(NULL) {}
  SingleIndicator(Wallet& wallet, Exchange& exchange, const char* nameAssets,
                  const char* nameIndicator, int avgtimes)
      : Strategy(wallet, exchange), m_pIndicator(NULL) {}
  virtual ~SingleIndicator() {}

 public:
  virtual void onTimeStamp(TimeStamp ts, int index);

 public:
  void initIndicator(const char* nameIndicator, int avgtimes);

 protected:
  std::string m_assetsName;
  Indicator* m_pIndicator;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__