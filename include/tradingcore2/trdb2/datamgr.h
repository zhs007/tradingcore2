#ifndef __TRADINGCORE2_TRDB2_DATAMGR_H__
#define __TRADINGCORE2_TRDB2_DATAMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

class TrDB2DataMgr {
 public:
  typedef std::map<std::string, tradingpb::Candles*> _Map;
  typedef std::pair<std::string, tradingpb::Candles*> _Pair;

 public:
  TrDB2DataMgr() {}
  ~TrDB2DataMgr() {}

 private:
  _Map m_map;
};

CR2END

#endif  // __TRADINGCORE2_TRDB2_DATAMGR_H__