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
    delete it->second;
  }

  this->m_map.clear();
}

// addData - add data
bool TrDB2DataMgr::addData(const char *market, const char *symbol,
                           std::vector<const char *> *pTags, int64_t tsStart,
                           int64_t tsEnd) {
  auto pCandles = new tradingpb::Candles();

  auto ret = getCandles(*pCandles, this->m_host.c_str(), this->m_token.c_str(),
                        market, symbol, pTags, tsStart, tsEnd);

  if (!ret) {
    return false;
  }

  std::string code = market;
  code += ".";
  code += symbol;

  _Pair p(code, pCandles);
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
    return it->second;
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
    auto candles = it->second;

    return tr2::getCandle(candles, ts);
  }

  return NULL;
}

void TrDB2DataMgr::foreachCandles(FuncOnCandles onCandles) {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    LOG(INFO) << "TrDB2DataMgr::foreachCandles " << it->first;

    onCandles(it->second);
  }
}

CR2END