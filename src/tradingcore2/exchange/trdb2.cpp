
#include <dirent.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/trdb2.h>
#include <tradingcore2/trdb2/utils.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

bool TrDB2Exchange::init(const Config& cfg) {
  this->rebuildTimeStampList(NULL);

  return true;
}

// const char* TrDB2CNFundsExchange::getTypeName() const {
//   return TrDB2CNFundsTypeName;
// }

// loadDat - [tsStart, tsEnd]
void TrDB2Exchange::loadData(const char* assetName, TimeStamp tsStart,
                             TimeStamp tsEnd, int offset) {
  this->m_mgrData.addData(this->getMarketName(), assetName, NULL, tsStart,
                          tsEnd, offset);
}

bool TrDB2Exchange::calculateVolume(const char* assetsName, TimeStamp ts,
                                    Money money, Volume& volume, Money& price,
                                    Money& fee, FuncCalcFee calcFee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(money > ZEROMONEY);

  auto c = this->m_mgrData.getCandle(this->getMarketName(), assetsName, ts);
  if (c == NULL) {
    return false;
  }

  if (c->close() <= 0) {
    return false;
  }

  volume = money / c->close();
  price = c->close();
  // fee = ZEROMONEY;

  if (calcFee != NULL) {
    fee = calcFee(assetsName, money, volume, ts);
  }

  return true;
}

bool TrDB2Exchange::calculateVolumeWithLimitPrice(const char* assetsName,
                                                  TimeStamp ts, Money money,
                                                  Volume& volume, Money& price,
                                                  Money& fee, Money limitPrice,
                                                  FuncCalcFee calcFee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(money > ZEROMONEY);

  auto c = this->m_mgrData.getCandle(this->getMarketName(), assetsName, ts);
  if (c == NULL) {
    return false;
  }

  if (c->low() > limitPrice) {
    return false;
  }

  if (c->high() < limitPrice) {
    limitPrice = c->high();
  }

  volume = money / limitPrice;
  price = limitPrice;
  // fee = ZEROMONEY;

  if (calcFee != NULL) {
    fee = calcFee(assetsName, money, volume, ts);
  }

  return true;
}

bool TrDB2Exchange::calculatePrice(const char* assetsName, TimeStamp ts,
                                   Volume volume, Money& money, Money& price,
                                   Money& fee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(volume > ZEROVOLUME);

  auto c = this->m_mgrData.getCandle(this->getMarketName(), assetsName, ts);
  if (c == NULL) {
    return false;
  }

  money = volume * c->close();
  price = c->close();
  fee = ZEROMONEY;

  return true;
}

bool TrDB2Exchange::calculatePriceWithLimitPrice(const char* assetsName,
                                                 TimeStamp ts, Volume volume,
                                                 Money& money, Money& price,
                                                 Money& fee, Money limitPrice,
                                                 FuncCalcFee calcFee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(volume > ZEROVOLUME);

  auto c = this->m_mgrData.getCandle(this->getMarketName(), assetsName, ts);
  if (c == NULL) {
    return false;
  }

  if (c->high() < limitPrice) {
    return false;
  }

  if (c->low() > limitPrice) {
    limitPrice = c->low();
  }

  money = volume * limitPrice;
  price = limitPrice;
  // fee = ZEROMONEY;

  if (calcFee != NULL) {
    fee = calcFee(assetsName, money, volume, ts);
  }

  return true;
}

bool TrDB2Exchange::getDataWithTimestamp(const char* assetsName, TimeStamp ts,
                                         CandleData& data) const {
  auto c = this->m_mgrData.getCandle2(this->getMarketName(), assetsName, ts);
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

bool TrDB2Exchange::getData(const char* assetsName, int index,
                            CandleData& data) {
  assert(index >= 0);

  auto candles = this->m_mgrData.getData(this->getMarketName(), assetsName);
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

int TrDB2Exchange::getDataLength(const char* assetsName) {
  auto candles = this->m_mgrData.getData(this->getMarketName(), assetsName);
  if (candles == NULL) {
    return 0;
  }

  return candles->candles_size();
}

void TrDB2Exchange::forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                     TimeStamp tsStart, TimeStamp tsEnd) const {
  if (this->m_lstTimeStamp.empty()) {
    return;
  }

  if (tsEnd <= 0) {
    tsEnd = this->m_lstTimeStamp.back();
  }

  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    if (*it > tsEnd) {
      break;
    }

    if (*it >= tsStart) {
      func(*this, *it, std::distance(this->m_lstTimeStamp.begin(), it));
    }
  }
}

void TrDB2Exchange::forEachAssetsData(const char* assetsName,
                                      Exchange::FuncOnAssetsData func,
                                      TimeStamp tsStart,
                                      TimeStamp tsEnd) const {
  auto candles = this->m_mgrData.getData(this->getMarketName(), assetsName);
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

void TrDB2Exchange::foreachCandlesTimeStamp(const tradingpb::Candles* candles) {
  for (auto i = 0; i < candles->candles_size(); ++i) {
    auto c = candles->candles(i);
    this->insertTimeStamp(c.ts());
  }
}

void TrDB2Exchange::rebuildTimeStampList(const char* assetsName) {
  this->m_lstTimeStamp.clear();

  auto f = std::bind(&TrDB2Exchange::foreachCandlesTimeStamp, this,
                     std::placeholders::_1);

  if (assetsName == NULL || strlen(assetsName) == 0) {
    this->m_mgrData.foreachCandles(f);
  } else {
    this->m_mgrData.foreachCandles2(this->getTypeName(), assetsName, f);
  }

  this->m_mgrData.buildMap();
}

void TrDB2Exchange::insertTimeStamp(TimeStamp ts) {
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

TimeStamp TrDB2Exchange::getLastTimeStamp() const {
  if (this->m_lstTimeStamp.empty()) {
    return 0;
  }

  return this->m_lstTimeStamp[this->m_lstTimeStamp.size() - 1];
}

TimeStamp TrDB2Exchange::getFirstTimeStamp() const {
  if (this->m_lstTimeStamp.empty()) {
    return 0;
  }

  return this->m_lstTimeStamp[0];
}

void TrDB2Exchange::release() { this->m_mgrData.release(); }

void TrDB2Exchange::mergeCandles(const tradingpb::Candles* candles) {
  this->m_mgrData.mergeCandles(candles);
}

// int TrDB2Exchange::getTradingDays4Year() const {
//   return this->m_mgrData.calcAverageTradingDays4Year();
// }

// Exchange* newTrDB2CNFunds(const Config& cfg) {
//   auto exchange =
//       new TrDB2CNFundsExchange(cfg.trdb2Serv.c_str(),
//       cfg.trdb2Token.c_str());

//   exchange->init(cfg);

//   return exchange;
// }

CR2END