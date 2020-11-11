#ifndef __TRADINGCORE2_TRNODE2_UTILS_H__
#define __TRADINGCORE2_TRNODE2_UTILS_H__

#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/tradingcore2.h>

CR2BEGIN

template <typename REQUEST, typename RESPONSE>
bool isValidTokens(const REQUEST* request, RESPONSE* response,
                   const Config& cfg) {
  auto token = request->basicrequest().token();
  return cfg.hasToken(token.c_str());
}

CR2END

#endif  // __TRADINGCORE2_TRNODE2_UTILS_H__