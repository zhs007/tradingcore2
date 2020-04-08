#ifndef __TRADINGCORE2_SRC_CLIENT_UTILS_H__
#define __TRADINGCORE2_SRC_CLIENT_UTILS_H__

#include <tradingcore2/tradingcore2.h>

#include "../proto/tradingcore2.grpc.pb.h"

CR2BEGIN

template <typename REQUEST>
void setRequest_Token(REQUEST* request, const char* token) {
  request->mutable_basicrequest()->set_token(token);
}

CR2END

#endif  // __TRADINGCORE2_SRC_CLIENT_UTILS_H__