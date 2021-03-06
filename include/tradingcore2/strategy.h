#ifndef __TRADINGCORE2_STRATEGY_H__
#define __TRADINGCORE2_STRATEGY_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

// Strategy - 仅处理单个资产，可以有多个条件，如果要处理多个资产，在外部处理
//    支持多个策略对应一个钱包
class Strategy {
 public:
  Strategy(Wallet& wallet, Exchange& exchange)
      : m_wallet(wallet),
        m_exchange(exchange),
        m_stoploss(-1),
        m_curStopLossPrice(-1),
        m_tradingNums(0),
        m_stoplossNums(0),
        m_failNums(0),
        m_handMoney(0),
        m_profitMoney(0),
        m_costMoney(0),
        m_initMoney(0),
        m_volume(0),
        m_price(0),
        m_fee(0),
        m_pCCData(NULL),
        m_lastAIPTs(0),
        m_lastMoneyParts(-1),
        m_lastTimesBuy(-1),
        m_lastTimesSell(-1),
        m_nextBuyStrategyID(0),
        m_nextBuyCtrlConditionID(0),
        m_nextSellStrategyID(0),
        m_nextSellCtrlConditionID(0),
        m_isFinish(false) {}
  virtual ~Strategy() { this->release(); }

 public:
  virtual void onTimeStamp(bool issim, TimeStamp ts, int index);

  virtual void onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                     Money fee, int offMoneyParts);

  virtual void onSell(bool issim, TimeStamp ts, Money money, Volume volume,
                      Money fee, int offMoneyParts, SellMoneyTo smt);

  virtual void onStopLoss(bool issim, TimeStamp ts, Money money, Volume volume,
                          Money fee, int offMoneyParts, SellMoneyTo smt);

  virtual void onTakeProfit(bool issim, TimeStamp ts, Money money,
                            Volume volume, Money fee, int offMoneyParts,
                            SellMoneyTo smt);

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

  const tradingpb::Strategy& getStrategy() const { return m_strategy; }

  void buy(bool issim, TimeStamp ts, int strategyID, int ctrlConditionID,
           bool noNextTimes);

  void sell(bool issim, TimeStamp ts, int strategyID, int ctrlConditionID,
            bool noNextTimes);

  void want2buy(bool issim, TimeStamp ts, Money money,
                Money limitPrice);

  void want2sell(bool issim, TimeStamp ts, Volume volume,
                 Money limitPrice);

  void takeProfit(bool issim, TimeStamp ts, int strategyID, int ctrlConditionID,
                  bool noNextTimes, CandleData& cd);

  void stopLoss(bool issim, TimeStamp ts, int strategyID, int ctrlConditionID,
                bool noNextTimes);

  void initMoney(bool issim, TimeStamp ts);

  void onAIP(bool issim, TimeStamp ts);

  const IndicatorMap& getMapIndicators() const { return this->m_mapIndicators; }

  Exchange& getExchange() { return this->m_exchange; }

  Wallet& getWallet() { return this->m_wallet; }

  void onNextTimes(bool issim, TimeStamp ts);

  void buildIndicators(const tradingpb::SimTradingParams& params,
                       PNL2& pnl2) const;

 protected:
  void release();

 protected:
  void onSimulateTradingTimeStamp(TimeStamp ts, int index);

  void setStopLossPrice(Money price) { m_curStopLossPrice = price; }

  Money onProcStopLoss(const char* assetsName, Money curPrice, Volume volume,
                       TimeStamp ts, int index, int strategyID,
                       int ctrlConditionID, int moneyParts);

  void onTradingFail() { m_failNums++; }

  void onTrading() { m_tradingNums++; }

  Money calcFeeForBuy();

  Money calcFee4Buy(const char* assetsName, Money money, Volume volume,
                    TimeStamp ts);

  Money calcFee4Sell(const char* assetsName, Money money, Volume volume,
                     TimeStamp ts);

  void nextBuy(int times, int strategyID, int ctrlConditionID, TimeStamp ts);

  void nextSell(int times, int strategyID, int ctrlConditionID, TimeStamp ts);

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
  Money m_handMoney;
  Money m_profitMoney;
  Money m_costMoney;
  Money m_initMoney;
  Volume m_volume;
  Money m_price;
  Money m_fee;
  CtrlConditionMgr::CtrlConditionData* m_pCCData;
  time_t m_lastAIPTs;
  IndicatorMap m_mapIndicators;
  int m_lastMoneyParts;

  int m_lastTimesBuy;
  int m_nextBuyStrategyID;
  int m_nextBuyCtrlConditionID;
  int m_lastTimesSell;
  int m_nextSellStrategyID;
  int m_nextSellCtrlConditionID;

  bool m_isFinish;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_H__