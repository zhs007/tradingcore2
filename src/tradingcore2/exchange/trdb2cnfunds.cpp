
#include <dirent.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/trdb2cnfunds.h>
#include <tradingcore2/trdb2/utils.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

// const CNFundValueNode* CNFundValue::getNode(TimeStamp ts) const {
//   for (auto it = this->data.begin(); it != this->data.end(); ++it) {
//     if (it->ts >= ts) {
//       return &(*it);
//     }
//   }

//   return NULL;
// }

bool TrDB2CNFundsExchange::init(const Config& cfg) {
  //   CNFundExchange* cnfund = this;
  //   auto onfile = [&cnfund](const char* dir, const char* fn) {
  //     cnfund->loadFundValue(joinPath(dir, fn).c_str());
  //   };

  //   tr2::foreachPathWithExt(cfg.cnfundPath.c_str(), ".csv", onfile);

  this->buildTimeStampList();

  return true;
}

const char* TrDB2CNFundsExchange::getTypeName() const {
  return TrDB2CNFundsTypeName;
}

// loadDat - [tsStart, tsEnd]
void TrDB2CNFundsExchange::loadData(const char* assetName, TimeStamp tsStart,
                                    TimeStamp tsEnd) {
  this->m_mgrData.addData("cnfunds", assetName, NULL, tsStart, tsEnd);
}

// void TrDB2CNFundsExchange::setFundValue(const char* assetsName,
//                                         CNFundValue* fv) {
//   this->releaseFundValue(assetsName);

//   Pair p;
//   p.first = assetsName;
//   p.second = fv;

//   auto itret = this->m_map.insert(p);
//   assert(itret.second);
// }

// void TrDB2CNFundsExchange::releaseFundValue(const char* assetsName) {
//   auto it = this->m_map.find(assetsName);
//   if (it != this->m_map.end()) {
//     delete it->second;

//     this->m_map.erase(it);
//   }
// }

// void TrDB2CNFundsExchange::loadFundValue(const char* fn) {
//   CSVLoader csv;
//   CNFundValue* fv = new CNFundValue();

//   csv.load(fn);

//   fv->code = csv.getData(0, 0);

//   for (int y = 0; y < csv.getHeight(); ++y) {
//     CNFundValueNode node;

//     node.value = strtoll(csv.getData(2, y), NULL, 10) / 10000.0;
//     node.totalValue = strtoll(csv.getData(3, y), NULL, 10) / 10000.0;
//     node.ts = str2timestampUTC(csv.getData(1, y), "%Y%m%d");

//     fv->data.push_back(node);
//   }

//   this->setFundValue(csv.getData(0, 0), fv);
// }

// const CNFundValue* TrDB2CNFundsExchange::getFundValue(
//     const char* assetsName) const {
//   auto it = m_map.find(assetsName);
//   if (it == m_map.end()) {
//     return NULL;
//   }

//   return it->second;
// }

bool TrDB2CNFundsExchange::calculateVolume(const char* assetsName, TimeStamp ts,
                                           Money money, Volume& volume,
                                           Money& price, Money& fee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(money > ZEROMONEY);

  auto c = this->m_mgrData.getCandle("cnfunds", assetsName, ts);
  if (c == NULL) {
    return false;
  }

  // auto fv = this->getFundValue(assetsName);
  // if (fv == NULL) {
  //   return false;
  // }

  // auto n = fv->getNode(ts);
  // if (n == NULL) {
  //   return false;
  // }

  volume = money / c->close();
  price = c->close();
  fee = ZEROMONEY;

  return true;
}

bool TrDB2CNFundsExchange::calculatePrice(const char* assetsName, TimeStamp ts,
                                          Volume volume, Money& money,
                                          Money& price, Money& fee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(volume > ZEROVOLUME);

  // auto fv = this->getFundValue(assetsName);
  // if (fv == NULL) {
  //   return false;
  // }

  // auto n = fv->getNode(ts);
  // if (n == NULL) {
  //   return false;
  // }

  auto c = this->m_mgrData.getCandle("cnfunds", assetsName, ts);
  if (c == NULL) {
    return false;
  }

  money = volume * c->close();
  price = c->close();
  fee = ZEROMONEY;

  return true;
}

bool TrDB2CNFundsExchange::getDataWithTimestamp(const char* assetsName,
                                                TimeStamp ts,
                                                CandleData& data) const {
  // auto fv = this->getFundValue(assetsName);
  // assert(fv != NULL);

  // auto n = fv->getNode(ts);
  // if (n == NULL) {
  //   return false;
  // }

  auto c = this->m_mgrData.getCandle("cnfunds", assetsName, ts);
  if (c == NULL) {
    return false;
  }

  data.ts = ts;
  data.open = data.close = data.high = data.low = c->close();
  data.volume = ZEROVOLUME;

  return true;
}

bool TrDB2CNFundsExchange::getData(const char* assetsName, int index,
                                   CandleData& data) {
  assert(index >= 0);

  // auto fv = this->getFundValue(assetsName);
  // assert(fv != NULL);

  auto candles = this->m_mgrData.getData("cnfunds", assetsName);
  if (candles == NULL) {
    return false;
  }

  if (index < candles->candles_size()) {
    auto c = candles->candles(index);
    data.ts = c.ts();
    data.open = data.close = data.high = data.low = c.close();
    data.volume = ZEROVOLUME;

    return true;
  }

  return false;
}

int TrDB2CNFundsExchange::getDataLength(const char* assetsName) {
  // auto fv = this->getFundValue(assetsName);
  // if (fv == NULL) {
  //   return 0;
  // }
  auto candles = this->m_mgrData.getData("cnfunds", assetsName);
  if (candles == NULL) {
    return 0;
  }

  return candles->candles_size();
}

void TrDB2CNFundsExchange::forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                            TimeStamp tsStart,
                                            TimeStamp tsEnd) const {
  if (tsEnd <= 0) {
    tsEnd = this->m_lstTimeStamp.back();
  }

  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    if (*it >= tsEnd) {
      break;
    }

    if (*it >= tsStart) {
      func(*this, *it, std::distance(this->m_lstTimeStamp.begin(), it));
    }
  }
}

void TrDB2CNFundsExchange::forEachAssetsData(const char* assetsName,
                                             Exchange::FuncOnAssetsData func,
                                             TimeStamp tsStart,
                                             TimeStamp tsEnd) const {
  auto candles = this->m_mgrData.getData("cnfunds", assetsName);
  assert(candles != NULL);

  // auto fv = this->getFundValue(assetsName);
  // assert(fv != NULL);

  if (tsStart == tsEnd) {
    for (auto i = 0; i < candles->candles_size(); ++i) {
      auto c = candles->candles(i);
      if (c.ts() > tsEnd) {
        break;
      }

      if (c.ts() >= tsStart) {
        CandleData cd(c.ts(), c.close());

        func(assetsName, &cd);
      }
    }

    return;
  }

  for (auto i = 0; i < candles->candles_size(); ++i) {
    auto c = candles->candles(i);
    if (c.ts() >= tsEnd) {
      break;
    }

    if (c.ts() >= tsStart) {
      CandleData cd(c.ts(), c.close());

      func(assetsName, &cd);
    }
  }
}

void TrDB2CNFundsExchange::foreachCandlesTimeStamp(
    const tradingpb::Candles* candles) {
  for (auto i = 0; i < candles->candles_size(); ++i) {
    auto c = candles->candles(i);
    this->insertTimeStamp(c.ts());
  }
}

void TrDB2CNFundsExchange::buildTimeStampList() {
  this->m_lstTimeStamp.clear();

  auto f = std::bind(&TrDB2CNFundsExchange::foreachCandlesTimeStamp, this,
                     std::placeholders::_1);

  this->m_mgrData.foreachCandles(f);
}

void TrDB2CNFundsExchange::insertTimeStamp(TimeStamp ts) {
  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    if (ts == *it) {
      return;
    }

    if (ts < *it) {
      this->m_lstTimeStamp.insert(it, ts);

      return;
    }
  }

  this->m_lstTimeStamp.push_back(ts);
}

TimeStamp TrDB2CNFundsExchange::getLastTimeStamp() const {
  assert(!this->m_lstTimeStamp.empty());

  return this->m_lstTimeStamp[this->m_lstTimeStamp.size() - 1];
}

TimeStamp TrDB2CNFundsExchange::getFirstTimeStamp() const {
  assert(!this->m_lstTimeStamp.empty());

  return this->m_lstTimeStamp[0];
}

void TrDB2CNFundsExchange::release() { this->m_mgrData.release(); }

Exchange* newTrDB2CNFunds(const Config& cfg) {
  auto exchange =
      new TrDB2CNFundsExchange(cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str());

  exchange->init(cfg);

  return exchange;
}

CR2END