#include <math.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

CtrlConditionMgr* CtrlConditionMgr::getSingleton() {
  // for c++ 11
  static CtrlConditionMgr s_mgr;

  return &s_mgr;
}

void CtrlConditionMgr::regCtrlCondition(const char* name,
                                        CtrlConditionHelper* cc) {
  PairCtrlCondition p(name, cc);

  auto ret = this->m_mapCtrlCondition.insert(p);
  assert(ret.second);
}

CtrlConditionHelper* CtrlConditionMgr::getCtrlCondition(const char* name) {
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second;
  }

  return NULL;
}

void CtrlConditionMgr::release() {
  for (auto it = this->m_mapCtrlCondition.begin();
       it != this->m_mapCtrlCondition.end(); ++it) {
    delete it->second;
  }

  this->m_mapCtrlCondition.clear();
}

// isValid - 检查 cc 协议的完整性，返回 0 表示有效，返回 1 表示没有支持这个
// cc，返回 -1 表示错误
int CtrlConditionMgr::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  auto name = cc.name();
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second->isValid(cc, ct) ? 0 : -1;
  }

  return 1;
}

// isValidStrategy - 检查 strategy 协议的完整性，返回 0 表示有效，返回 1
// 表示没有有未支持的 CtrlCondition，返回 -1 表示有错误
int CtrlConditionMgr::isValidStrategy(const tradingpb::Strategy& strategy) {
  bool hasnocc = false;

  for (auto i = 0; i < strategy.buy_size(); i++) {
    auto cc = strategy.buy(i);
    auto ret = this->isValid(cc, CT_BUY);
    if (ret == -1) {
      return -1;
    } else if (ret == 1) {
      hasnocc = true;
    }
  }

  for (auto i = 0; i < strategy.sell_size(); i++) {
    auto cc = strategy.sell(i);
    auto ret = this->isValid(cc, CT_SELL);
    if (ret == -1) {
      return -1;
    } else if (ret == 1) {
      hasnocc = true;
    }
  }

  for (auto i = 0; i < strategy.stoploss_size(); i++) {
    auto cc = strategy.stoploss(i);
    auto ret = this->isValid(cc, CT_STOPLOSS);
    if (ret == -1) {
      return -1;
    } else if (ret == 1) {
      hasnocc = true;
    }
  }

  for (auto i = 0; i < strategy.takeprofit_size(); i++) {
    auto cc = strategy.takeprofit(i);
    auto ret = this->isValid(cc, CT_TAKEPROFIT);
    if (ret == -1) {
      return -1;
    } else if (ret == 1) {
      hasnocc = true;
    }
  }

  if (hasnocc) {
    return 1;
  }

  return 0;
}

bool CtrlConditionMgr::canCtrl(const Exchange& exchange, const Wallet& wallet,
                               const IndicatorMap& mapIndicators,
                               const tradingpb::CtrlCondition& cc, bool issim,
                               CtrlType ct, TimeStamp ts, int index,
                               CandleData& cd, Assets& assets, void* pData) {
  auto name = cc.name();
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second->canCtrl(exchange, wallet, mapIndicators, cc, issim, ct,
                               ts, index, cd, assets, pData);
  }

  return false;
}

bool CtrlConditionMgr::canCtrlInCtrlConditions(
    const Exchange& exchange, const Wallet& wallet,
    const IndicatorMap& mapIndicators, int ccnums, bool issim, CtrlType ct,
    TimeStamp ts, int index, CandleData& cd, Assets& assets,
    FuncGetCtrlCondition funcGetCC, int& ctrlConditionID) {
  ctrlConditionID = 0;
  // LOG(INFO) << "ccnums " << ccnums;
  std::map<int, bool> mapGroup;

  for (auto i = 0; i < ccnums; i++) {
    const ::tradingpb::CtrlCondition* pCC = NULL;
    void* pData = NULL;

    funcGetCC(i, &pCC, &pData);

    bool isNewGroup = false;
    auto itGroup = mapGroup.find(pCC->group());
    if (itGroup == mapGroup.end()) {
      std::pair<int, bool> pairGroup(pCC->group(), false);
      mapGroup.insert(pairGroup);

      itGroup = mapGroup.find(pCC->group());
      isNewGroup = true;
    }

    bool curcanctrl = this->canCtrl(exchange, wallet, mapIndicators, *pCC,
                                    issim, ct, ts, index, cd, assets, pData);
    if (isNewGroup) {
      itGroup->second = curcanctrl;
    } else {
      if (pCC->combcondition() == "||") {
        itGroup->second = itGroup->second || curcanctrl;
      } else {
        itGroup->second = itGroup->second && curcanctrl;
      }
    }

    // LOG(INFO) << "canCtrl ct " << ct << " group " << pCC->group() << "
    // canctrl "
    //           << curcanctrl << " ctrl " << itGroup->second;
  }

  if (mapGroup.size() == 1) {
    if (mapGroup.begin()->second) {
      return true;
    }
  } else {
    bool canctrl = false;
    for (auto it = mapGroup.begin(); it != mapGroup.end(); ++it) {
      ctrlConditionID++;

      if (it->second) {
        canctrl = true;

        break;
      }
    }

    if (canctrl) {
      return true;
    }
  }

  return false;
}

// procStrategy -
int CtrlConditionMgr::procStrategy(Strategy& strategy,
                                   CtrlConditionMgr::CtrlConditionData* pData,
                                   bool issim, TimeStamp ts, int index) {
  auto pbStrategy = strategy.getStrategy();
  Exchange& exchange = strategy.getExchange();
  Wallet& wallet = strategy.getWallet();
  CandleData cd;
  exchange.getData(pbStrategy.asset().code().c_str(), index, cd);
  Assets assets = wallet.getAssets(pbStrategy.asset().code().c_str());

  strategy.onNextTimes(issim, ts);

  bool canstoploss = false;
  bool cantakeprofit = false;
  int stoplossConditionID = 0;
  int takeprofitConditionID = 0;

  {
    auto f0 = [](const tradingpb::Strategy& pbStrategy,
                 CtrlConditionMgr::CtrlConditionData* pData, int i,
                 const ::tradingpb::CtrlCondition** ppCC, void** ppD) {
      *ppCC = &pbStrategy.stoploss(i);
      *ppD = pData->lstStopLoss[i];
    };
    auto f = std::bind(f0, pbStrategy, pData, std::placeholders::_1,
                       std::placeholders::_2, std::placeholders::_3);

    if (this->canCtrlInCtrlConditions(
            exchange, wallet, strategy.getMapIndicators(),
            pbStrategy.stoploss_size(), issim, CT_STOPLOSS, ts, index, cd,
            assets, f, stoplossConditionID)) {
      canstoploss = true;
    }
  }

  {
    auto f0 = [](const tradingpb::Strategy& pbStrategy,
                 CtrlConditionMgr::CtrlConditionData* pData, int i,
                 const ::tradingpb::CtrlCondition** ppCC, void** ppD) {
      *ppCC = &pbStrategy.takeprofit(i);
      *ppD = pData->lstTakeProfit[i];
    };
    auto f = std::bind(f0, pbStrategy, pData, std::placeholders::_1,
                       std::placeholders::_2, std::placeholders::_3);

    if (this->canCtrlInCtrlConditions(
            exchange, wallet, strategy.getMapIndicators(),
            pbStrategy.takeprofit_size(), issim, CT_TAKEPROFIT, ts, index, cd,
            assets, f, takeprofitConditionID)) {
      cantakeprofit = true;
    }
  }

  if (cantakeprofit) {
    strategy.takeProfit(issim, ts, 0, takeprofitConditionID, false);

    return 0;
  } else if (canstoploss) {
    strategy.stopLoss(issim, ts, 0, stoplossConditionID, false);

    return 0;
  }

  bool canbuy = false;
  bool cansell = false;
  int buyConditionID = 0;
  int sellConditionID = 0;

  {
    auto f0 = [](const tradingpb::Strategy& pbStrategy,
                 CtrlConditionMgr::CtrlConditionData* pData, int i,
                 const ::tradingpb::CtrlCondition** ppCC, void** ppD) {
      // LOG(INFO) << "i " << i;

      *ppCC = &pbStrategy.buy(i);
      *ppD = pData->lstBuy[i];
    };
    auto f = std::bind(f0, pbStrategy, pData, std::placeholders::_1,
                       std::placeholders::_2, std::placeholders::_3);

    if (this->canCtrlInCtrlConditions(exchange, wallet,
                                      strategy.getMapIndicators(),
                                      pbStrategy.buy_size(), issim, CT_BUY, ts,
                                      index, cd, assets, f, buyConditionID)) {
      // LOG(INFO) << "buy " << buyConditionID;

      // strategy.buy(issim, ts);
      canbuy = true;
    }
  }

  {
    auto f0 = [](const tradingpb::Strategy& pbStrategy,
                 CtrlConditionMgr::CtrlConditionData* pData, int i,
                 const ::tradingpb::CtrlCondition** ppCC, void** ppD) {
      // LOG(INFO) << "i " << i;

      *ppCC = &pbStrategy.sell(i);
      *ppD = pData->lstSell[i];
    };
    auto f = std::bind(f0, pbStrategy, pData, std::placeholders::_1,
                       std::placeholders::_2, std::placeholders::_3);

    if (this->canCtrlInCtrlConditions(
            exchange, wallet, strategy.getMapIndicators(),
            pbStrategy.sell_size(), issim, CT_SELL, ts, index, cd, assets, f,
            sellConditionID)) {
      // LOG(INFO) << "sell " << sellConditionID;

      // strategy.sell(issim, ts);
      cansell = true;
    }
  }

  if (canbuy && !cansell) {
    strategy.buy(issim, ts, 0, buyConditionID, false);
  } else if (cansell && !canbuy) {
    strategy.sell(issim, ts, 0, sellConditionID, false);
  }

  return 0;
}

void CtrlConditionMgr::getIndicators(std::set<std::string>& indicators,
                                     Strategy& strategy) {
  auto pbStrategy = strategy.getStrategy();

  {
    for (auto i = 0; i < pbStrategy.buy_size(); i++) {
      auto cc = pbStrategy.buy(i);

      auto name = cc.name();
      auto it = this->m_mapCtrlCondition.find(name);
      if (it != this->m_mapCtrlCondition.end()) {
        it->second->getIndicators(indicators, cc);
      }
    }
  }

  {
    for (auto i = 0; i < pbStrategy.sell_size(); i++) {
      auto cc = pbStrategy.sell(i);

      auto name = cc.name();
      auto it = this->m_mapCtrlCondition.find(name);
      if (it != this->m_mapCtrlCondition.end()) {
        it->second->getIndicators(indicators, cc);
      }
    }
  }

  {
    for (auto i = 0; i < pbStrategy.stoploss_size(); i++) {
      auto cc = pbStrategy.stoploss(i);

      auto name = cc.name();
      auto it = this->m_mapCtrlCondition.find(name);
      if (it != this->m_mapCtrlCondition.end()) {
        it->second->getIndicators(indicators, cc);
      }
    }
  }

  {
    for (auto i = 0; i < pbStrategy.takeprofit_size(); i++) {
      auto cc = pbStrategy.takeprofit(i);

      auto name = cc.name();
      auto it = this->m_mapCtrlCondition.find(name);
      if (it != this->m_mapCtrlCondition.end()) {
        it->second->getIndicators(indicators, cc);
      }
    }
  }
}

void* CtrlConditionMgr::newCtrlConditionData(
    const tradingpb::CtrlCondition& cc) {
  auto name = cc.name();
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second->newCtrlConditionData();
  }

  return NULL;
}

void CtrlConditionMgr::deleteCtrlConditionData(
    const tradingpb::CtrlCondition& cc, void* pData) {
  if (pData == NULL) {
    return;
  }

  auto name = cc.name();
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    it->second->deleteCtrlConditionData(pData);
  }
}

CtrlConditionMgr::CtrlConditionData* CtrlConditionMgr::newCtrlConditionData(
    Strategy& strategy) {
  auto pData = new CtrlConditionMgr::CtrlConditionData();

  auto pbStrategy = strategy.getStrategy();

  for (auto i = 0; i < pbStrategy.buy_size(); i++) {
    auto cc = pbStrategy.buy(i);
    auto pD = this->newCtrlConditionData(cc);
    pData->lstBuy.push_back(pD);
  }

  for (auto i = 0; i < pbStrategy.sell_size(); i++) {
    auto cc = pbStrategy.sell(i);
    auto pD = this->newCtrlConditionData(cc);
    pData->lstSell.push_back(pD);
  }

  for (auto i = 0; i < pbStrategy.stoploss_size(); i++) {
    auto cc = pbStrategy.stoploss(i);
    auto pD = this->newCtrlConditionData(cc);
    pData->lstStopLoss.push_back(pD);
  }

  for (auto i = 0; i < pbStrategy.takeprofit_size(); i++) {
    auto cc = pbStrategy.takeprofit(i);
    auto pD = this->newCtrlConditionData(cc);
    pData->lstTakeProfit.push_back(pD);
  }

  return pData;
}

void CtrlConditionMgr::deleteCtrlConditionData(
    Strategy& strategy, CtrlConditionMgr::CtrlConditionData* pData) {
  if (pData == NULL) {
    return;
  }

  auto pbStrategy = strategy.getStrategy();

  for (auto i = 0; i < pbStrategy.buy_size(); i++) {
    auto cc = pbStrategy.buy(i);
    auto pD = pData->lstBuy[i];
    this->deleteCtrlConditionData(cc, pD);
  }

  pData->lstBuy.clear();

  for (auto i = 0; i < pbStrategy.sell_size(); i++) {
    auto cc = pbStrategy.sell(i);
    auto pD = pData->lstSell[i];
    this->deleteCtrlConditionData(cc, pD);
  }

  pData->lstSell.clear();

  for (auto i = 0; i < pbStrategy.stoploss_size(); i++) {
    auto cc = pbStrategy.stoploss(i);
    auto pD = pData->lstStopLoss[i];
    this->deleteCtrlConditionData(cc, pD);
  }

  pData->lstStopLoss.clear();

  for (auto i = 0; i < pbStrategy.takeprofit_size(); i++) {
    auto cc = pbStrategy.takeprofit(i);
    auto pD = pData->lstTakeProfit[i];
    this->deleteCtrlConditionData(cc, pD);
  }

  pData->lstTakeProfit.clear();

  delete pData;
}

CR2END