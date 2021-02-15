#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/client/train2.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trdb2/client.h>
#include <tradingcore2/trdb2/datamgr.h>
#include <tradingcore2/trdb2/utils.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

void TrDB2DataMgr::release() {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    delete it->second.candles;
  }

  this->m_map.clear();
}

// addData - add data
bool TrDB2DataMgr::addData(const char *market, const char *symbol,
                           std::vector<const char *> *pTags, int64_t tsStart,
                           int64_t tsEnd, int32_t offset) {
  auto pCandles = new tradingpb::Candles();

  auto ret = getCandles(*pCandles, this->m_host.c_str(), this->m_token.c_str(),
                        market, symbol, pTags, tsStart, tsEnd, offset);

  if (!ret) {
    return false;
  }

  std::string code = market;
  code += ".";
  code += symbol;

  _Pair p;
  p.first = code;
  p.second.candles = pCandles;
  auto retIns = this->m_map.insert(p);

  return retIns.second;
}

const tradingpb::Candles *TrDB2DataMgr::getData(const char *market,
                                                const char *symbol) const {
  std::string code = market;
  code += ".";
  code += symbol;

  auto it = this->m_map.find(code);
  if (it != this->m_map.end()) {
    return it->second.candles;
  }

  return NULL;
}

const tradingpb::Candle *TrDB2DataMgr::getCandle(const char *market,
                                                 const char *symbol,
                                                 int64_t ts) const {
  std::string code = market;
  code += ".";
  code += symbol;

  auto it = this->m_map.find(code);
  if (it != this->m_map.end()) {
    auto candles = it->second.candles;

    return tr2::getCandle(candles, ts);
  }

  return NULL;
}

const tradingpb::Candle *TrDB2DataMgr::getCandle2(const char *market,
                                                  const char *symbol,
                                                  int64_t ts) const {
  std::string code = market;
  code += ".";
  code += symbol;

  auto it = this->m_map.find(code);
  if (it != this->m_map.end()) {
    auto cit = it->second.mapCandles.find(ts);
    if (cit != it->second.mapCandles.end()) {
      return cit->second;
    }
  }

  return NULL;
}

void TrDB2DataMgr::foreachCandles(FuncOnCandles onCandles) {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    LOG(INFO) << "TrDB2DataMgr::foreachCandles " << it->first;

    onCandles(it->second.candles);
  }
}

void TrDB2DataMgr::foreachCandles2(const char *market, const char *symbol,
                                   FuncOnCandles onCandles) {
  std::string code = market;
  code += ".";
  code += symbol;

  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    LOG(INFO) << "TrDB2DataMgr::foreachCandles2 " << it->first;

    if (it->first == code) {
      onCandles(it->second.candles);
    }
  }
}

int TrDB2DataMgr::getTradingDays4Year(const char *market,
                                      const char *symbol) const {
  std::string code = market;
  code += ".";
  code += symbol;

  auto it = this->m_map.find(code);
  if (it != this->m_map.end()) {
    return calcTradingDays4Year(*it->second.candles);
  }

  return 0;
}

int TrDB2DataMgr::calcAverageTradingDays4Year() const {
  int totaldfy = 0;
  int tnums = 0;
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    auto dfy = calcTradingDays4Year(*it->second.candles);
    if (dfy > 0) {
      totaldfy += dfy;
      tnums++;
    }
  }

  if (tnums <= 1) {
    return totaldfy;
  }

  return totaldfy / tnums;
}

void TrDB2DataMgr::buildMap() {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    it->second.mapCandles.clear();

    for (auto i = 0; i < it->second.candles->candles_size(); ++i) {
      auto mc = it->second.candles->mutable_candles(i);
      std::pair<int64_t, const tradingpb::Candle *> p(mc->ts(), mc);
      it->second.mapCandles.insert(p);
    }
  }
}

CR2END