#ifndef __TRADINGCORE2_TRDB2_CLIENT_H__
#define __TRADINGCORE2_TRDB2_CLIENT_H__

#include <tradingcore2/basedef.h>

CR2BEGIN

// getCandles - get candles
bool getCandles(tradingdb2pb::Candles &candles, const char *host,
                const char *token, const char *market, const char *symbol,
                const char *tag);

CR2END

#endif  // __TRADINGCORE2_TRDB2_CLIENT_H__