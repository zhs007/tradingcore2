#include <math.h>
#include <tradingcore2/ctrlcondition.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>

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

  if (hasnocc) {
    return 1;
  }

  return 0;
}

bool CtrlConditionMgr::canCtrl(const IndicatorMap& mapIndicators,
                               const tradingpb::CtrlCondition& cc, bool issim,
                               CtrlType ct, TimeStamp ts, int index,
                               CandleData& cd, void* pData) {
  auto name = cc.name();
  auto it = this->m_mapCtrlCondition.find(name);
  if (it != this->m_mapCtrlCondition.end()) {
    return it->second->canCtrl(mapIndicators, cc, issim, ct, ts, index, cd,
                               pData);
  }

  return false;
}

bool CtrlConditionMgr::canCtrl(const IndicatorMap& mapIndicators, int ccnums,
                               bool issim, CtrlType ct, TimeStamp ts, int index,
                               CandleData& cd, FuncGetCtrlCondition funcGetCC) {
  // LOG(INFO) << "ccnums " << ccnums;
  std::map<int, bool> mapGroup;

  for (auto i = 0; i < ccnums; i++) {
    const ::tradingpb::CtrlCondition* pCC = NULL;
    void* pData = NULL;

    funcGetCC(i, &pCC, &pData);

    auto itGroup = mapGroup.find(pCC->group());
    if (itGroup == mapGroup.end()) {
      std::pair pairGroup(pCC->group(), false);
      mapGroup.insert(pairGroup);

      itGroup = mapGroup.find(pCC->group());
    }

    bool curcanctrl =
        this->canCtrl(mapIndicators, *pCC, issim, ct, ts, index, cd, pData);
    if (i == 0) {
      itGroup->second = curcanctrl;
    } else {
      if (pCC->combcondition() == "||") {
        itGroup->second = itGroup->second || curcanctrl;
      } else {
        itGroup->second = itGroup->second && curcanctrl;
      }
    }
  }

  if (mapGroup.size() == 1) {
    if (mapGroup.begin()->second) {
      return true;
    }
  } else {
    bool canctrl = false;
    for (auto it = mapGroup.begin(); it != mapGroup.end(); ++it) {
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
  CandleData cd;
  exchange.getData(pbStrategy.asset().code().c_str(), index, cd);

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

    if (this->canCtrl(strategy.getMapIndicators(), pbStrategy.buy_size(), issim,
                      CT_BUY, ts, index, cd, f)) {
      strategy.buy(issim, ts);
    }

    // // bool canbuy = false;
    // std::map<int, bool> mapGroup;

    // for (auto i = 0; i < pbStrategy.buy_size(); i++) {
    //   auto cc = pbStrategy.buy(i);
    //   auto pD = pData->lstBuy[i];

    //   auto itGroup = mapGroup.find(cc.group());
    //   if (itGroup == mapGroup.end()) {
    //     std::pair pairGroup(cc.group(), false);
    //     mapGroup.insert(pairGroup);

    //     itGroup = mapGroup.find(cc.group());
    //   }

    //   bool curcanctrl = this->canCtrl(strategy.getMapIndicators(), cc, issim,
    //                                   CT_BUY, ts, index, cd, pD);
    //   if (i == 0) {
    //     itGroup->second = curcanctrl;
    //   } else {
    //     if (cc.combcondition() == "||") {
    //       itGroup->second = itGroup->second || curcanctrl;
    //     } else {
    //       itGroup->second = itGroup->second && curcanctrl;
    //     }
    //   }
    // }

    // if (mapGroup.size() == 1) {
    //   if (mapGroup.begin()->second) {
    //     strategy.buy(issim, ts);
    //   }
    // } else {
    //   bool canbuy = false;
    //   for (auto it = mapGroup.begin(); it != mapGroup.end(); ++it) {
    //     if (it->second) {
    //       canbuy = true;

    //       break;
    //     }
    //   }

    //   if (canbuy) {
    //     strategy.buy(issim, ts);
    //   }
    // }
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

    if (this->canCtrl(strategy.getMapIndicators(), pbStrategy.sell_size(),
                      issim, CT_SELL, ts, index, cd, f)) {
      strategy.sell(issim, ts);
    }
    // // bool cansell = false;
    // std::map<int, bool> mapGroup;

    // for (auto i = 0; i < pbStrategy.sell_size(); i++) {
    //   auto cc = pbStrategy.sell(i);
    //   auto pD = pData->lstSell[i];

    //   auto itGroup = mapGroup.find(cc.group());
    //   if (itGroup == mapGroup.end()) {
    //     std::pair pairGroup(cc.group(), false);
    //     mapGroup.insert(pairGroup);

    //     itGroup = mapGroup.find(cc.group());
    //   }

    //   bool curcanctrl = this->canCtrl(strategy.getMapIndicators(), cc, issim,
    //                                   CT_SELL, ts, index, cd, pD);
    //   if (i == 0) {
    //     itGroup->second = curcanctrl;
    //   } else {
    //     if (cc.combcondition() == "||") {
    //       itGroup->second = itGroup->second || curcanctrl;
    //     } else {
    //       itGroup->second = itGroup->second && curcanctrl;
    //     }
    //   }
    // }

    // if (mapGroup.size() == 1) {
    //   if (mapGroup.begin()->second) {
    //     strategy.sell(issim, ts);
    //   }
    // } else {
    //   bool cansell = false;
    //   for (auto it = mapGroup.begin(); it != mapGroup.end(); ++it) {
    //     if (it->second) {
    //       cansell = true;

    //       break;
    //     }
    //   }

    //   if (cansell) {
    //     strategy.sell(issim, ts);
    //   }
    // }
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

  delete pData;
}

CR2END