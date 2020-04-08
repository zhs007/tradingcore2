#ifndef __TRADINGCORE2_SRC_SERVER_UTILS_H__
#define __TRADINGCORE2_SRC_SERVER_UTILS_H__

#include <tradingcore2/basedef.h>

#include "../proto/tradingcore2.grpc.pb.h"

CR2BEGIN

template <typename T>
void setResponse_ErrorCode(T* response, tradingcore2pb::ErrorCode code) {
  response->mutable_basicreply()->set_errcode(code);
}

CR2END

#endif  // __TRADINGCORE2_SRC_SERVER_UTILS_H__