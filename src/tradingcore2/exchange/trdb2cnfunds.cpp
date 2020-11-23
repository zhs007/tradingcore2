
#include <dirent.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/trdb2cnfunds.h>
#include <tradingcore2/trdb2/utils.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

bool TrDB2CNFundsExchange::init(const Config& cfg) {
  this->rebuildTimeStampList();

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
  auto c = this->m_mgrData.getCandle("cnfunds", assetsName, ts);
  if (c == NULL) {
    // LOG(INFO) << "TrDB2CNFundsExchange:getDataWithTimestamp " << assetsName
    //           << " " << ts;

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

void TrDB2CNFundsExchange::rebuildTimeStampList() {
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
  if (this->m_lstTimeStamp.empty()) {
    return 0;
  }

  return this->m_lstTimeStamp[this->m_lstTimeStamp.size() - 1];
}

TimeStamp TrDB2CNFundsExchange::getFirstTimeStamp() const {
  if (this->m_lstTimeStamp.empty()) {
    return 0;
  }

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