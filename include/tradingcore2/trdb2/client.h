#ifndef __TRADINGCORE2_TRDB2_CLIENT_H__
#define __TRADINGCORE2_TRDB2_CLIENT_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/proto/tradingdb2.grpc.pb.h>

CR2BEGIN

// getCandles - get candles
bool getCandles(tradingdb2pb::Candles &candles, const char *host,
                const char *token, const char *market, const char *symbol,
                std::vector<const char *>* pTags, int64_t tsStart, int64_t tsEnd);

CR2END

#endif  // __TRADINGCORE2_TRDB2_CLIENT_H__