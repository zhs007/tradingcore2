#ifndef __TRADINGCORE2_TRDB2_UTILS_H__
#define __TRADINGCORE2_TRDB2_UTILS_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

const tradingpb::Candle *getCandle(const tradingpb::Candles *candles,
                                   int64_t ts);

CR2END

#endif  // __TRADINGCORE2_TRDB2_UTILS_H__