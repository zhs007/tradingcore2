#ifndef __TRADINGCORE2_STRATEGY_H__
#define __TRADINGCORE2_STRATEGY_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

// Strategy - 仅处理单个资产，可以有多个条件，如果要处理多个资产，在外部处理
class Strategy {
 public:
  typedef std::function<void(bool, TimeStamp, int, const tradingpb::Asset*,
                             Money, Volume)>
      FuncOnCtrl;

 public:
  Strategy(Wallet& wallet, Exchange& exchange)
      : m_wallet(wallet),
        m_exchange(exchange),
        m_stoploss(-1),
        m_curStopLossPrice(-1),
        m_tradingNums(0),
        m_stoplossNums(0),
        m_failNums(0) {}
  virtual ~Strategy() {}

 public:
  virtual void onTimeStamp(bool issim, TimeStamp ts, int index);

  virtual void onBuy(bool issim, TimeStamp ts, int index,
                     const tradingpb::Asset* pAsset, Money money) = 0;

 public:
  void init(const tradingpb::Strategy& strategy) {
    m_strategy.CopyFrom(strategy);
  }

 public:
  void setStopLess(float stoploss) { m_stoploss = stoploss; }

 public:
  void simulateTrading();

  void print();

  void getTrainResult(TrainResult& tr);

 protected:
  void onSimulateTradingTimeStamp(TimeStamp ts, int index);

  void setStopLossPrice(Money price) { m_curStopLossPrice = price; }

  Money onProcStopLoss(const char* assetsName, Money curPrice, Volume volume,
                       TimeStamp ts, int index);

  void onTradingFail() { m_failNums++; }

  void onTrading() { m_tradingNums++; }

 protected:
  void onCtrlConditionBuy(bool issim, TimeStamp ts, int index);

  void onWeekDay(const tradingpb::CtrlCondition& cc, bool issim, TimeStamp ts,
                 int index, const tradingpb::Asset* pAsset,
                 Strategy::FuncOnCtrl onctrl);

  void onMonthDay(const tradingpb::CtrlCondition& cc, bool issim, TimeStamp ts,
                  int index, const tradingpb::Asset* pAsset,
                  Strategy::FuncOnCtrl onctrl);

  void onBuyAndHold(const tradingpb::CtrlCondition& cc, bool issim,
                    TimeStamp ts, int index, const tradingpb::Asset* pAsset,
                    Strategy::FuncOnCtrl onctrl);

 protected:
  Wallet& m_wallet;
  Exchange& m_exchange;
  PNL m_pnl;

  float m_stoploss;
  Money m_curStopLossPrice;

  int m_tradingNums;
  int m_stoplossNums;
  int m_failNums;

  tradingpb::Strategy m_strategy;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_H__