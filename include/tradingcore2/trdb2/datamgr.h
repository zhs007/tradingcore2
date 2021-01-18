#ifndef __TRADINGCORE2_TRDB2_DATAMGR_H__
#define __TRADINGCORE2_TRDB2_DATAMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

typedef std::function<void(const tradingpb::Candles *)> FuncOnCandles;

class TrDB2DataMgr {
 public:
  struct CandlesData {
    tradingpb::Candles *candles;
    std::map<int64_t, const tradingpb::Candle *> mapCandles;
  };

  typedef std::map<std::string, CandlesData> _Map;
  typedef std::pair<std::string, CandlesData> _Pair;

 public:
  TrDB2DataMgr(const char *host, const char *token)
      : m_host(host), m_token(token) {}
  ~TrDB2DataMgr() { this->release(); }

 public:
  void release();

 public:
  // addData - add data
  bool addData(const char *market, const char *symbol,
               std::vector<const char *> *pTags, int64_t tsStart,
               int64_t tsEnd);

  const tradingpb::Candles *getData(const char *market,
                                    const char *symbol) const;

  const tradingpb::Candle *getCandle(const char *market, const char *symbol,
                                     int64_t ts) const;

  const tradingpb::Candle *getCandle2(const char *market, const char *symbol,
                                      int64_t ts) const;

  void foreachCandles(FuncOnCandles onCandles);

  void foreachCandles2(const char *market, const char *symbol,
                       FuncOnCandles onCandles);

  int getTradingDays4Year(const char *market, const char *symbol) const;

  int calcAverageTradingDays4Year() const;

  void buildMap();

 private:
  _Map m_map;

  std::string m_host;
  std::string m_token;
};

CR2END

#endif  // __TRADINGCORE2_TRDB2_DATAMGR_H__