#ifndef __TRADINGCORE2_TRNODE2_UTILS_H__
#define __TRADINGCORE2_TRNODE2_UTILS_H__

#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/tradingcore2.h>

CR2BEGIN

// isValidTokens - check token
template <typename REQUEST, typename RESPONSE>
bool isValidTokens(const REQUEST* request, RESPONSE* response,
                   const Config& cfg) {
  auto token = request->basicrequest().token();
  return cfg.hasToken(token.c_str());
}

// findAssetData - find PNLAssetData
::tradingpb::PNLAssetData* findAssetData(::tradingpb::PNLData* pPNLData,
                                         const char* market,
                                         const char* symbol);

// findAssetDataEx - find PNLAssetData
//      if can't find it, it will new a PNLAssetData
::tradingpb::PNLAssetData* findAssetDataEx(::tradingpb::PNLData* pPNLData,
                                           const char* market,
                                           const char* symbol);

// insTimestamp - insert a timestamp
void insTimestamp(::tradingpb::PNLAssetData* pAssetData, time_t ts);

CR2END

#endif  // __TRADINGCORE2_TRNODE2_UTILS_H__