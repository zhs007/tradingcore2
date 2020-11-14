#ifndef __TRADINGCORE2_TRDB2_DATAMGR_H__
#define __TRADINGCORE2_TRDB2_DATAMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

class TrDB2DataMgr {
 public:
  typedef std::map<std::string, tradingpb::Candles *> _Map;
  typedef std::pair<std::string, tradingpb::Candles *> _Pair;

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

 private:
  _Map m_map;

  std::string m_host;
  std::string m_token;
};

CR2END

#endif  // __TRADINGCORE2_TRDB2_DATAMGR_H__