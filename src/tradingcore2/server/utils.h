#ifndef __TRADINGCORE2_SRC_SERVER_UTILS_H__
#define __TRADINGCORE2_SRC_SERVER_UTILS_H__

#include <tradingcore2/protos/tradingcore2.grpc.pb.h>
#include <tradingcore2/tradingcore2.h>

CR2BEGIN

template <typename RESPONSE>
void setResponse_ErrorCode(RESPONSE* response, tradingcore2pb::ErrorCode code) {
  response->mutable_basicreply()->set_errcode(code);
}

template <typename REQUEST, typename RESPONSE>
bool isValidTokens(const REQUEST* request, RESPONSE* response,
                   const Config& cfg) {
  auto token = request->basicrequest().token();
  if (!cfg.hasToken(token.c_str())) {
    setResponse_ErrorCode(response, tradingcore2pb::ERR_INVALIDTOKEN);

    return false;
  }

  return true;
}

void setTradeData(tradingcore2pb::TradeData& td, const WalletHistoryNode& node);

void setTradeHistory(tradingcore2pb::TrainNodeResult& tnr,
                     const Wallet& wallet);

CR2END

#endif  // __TRADINGCORE2_SRC_SERVER_UTILS_H__