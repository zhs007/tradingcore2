#ifndef __TRADINGCORE2_SRC_CLIENT_UTILS_H__
#define __TRADINGCORE2_SRC_CLIENT_UTILS_H__

#include <tradingcore2/proto/tradingcore2.grpc.pb.h>
#include <tradingcore2/tradingcore2.h>

CR2BEGIN

template <typename REQUEST>
void setRequest_Token(REQUEST* request, const char* token) {
  request->mutable_basicrequest()->set_token(token);
}

void setTradeHistory2TrainResult(TrainResult& tr,
                                 const tradingcore2pb::TrainNodeResult& tnr);

CR2END

#endif  // __TRADINGCORE2_SRC_CLIENT_UTILS_H__