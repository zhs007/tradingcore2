#ifndef __TRADINGCORE2_TRDB2_CLIENT_H__
#define __TRADINGCORE2_TRDB2_CLIENT_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

typedef std::function<void(tradingpb::SymbolInfo &si)> FuncOnSymbol;

// getCandles - get candles
bool getCandles(tradingpb::Candles &candles, const char *host,
                const char *token, const char *market, const char *symbol,
                std::vector<const char *> *pTags, int64_t tsStart,
                int64_t tsEnd, int32_t offset);

// getSymbols - get symbols
bool getSymbols(const char *host, const char *token, const char *market,
                std::vector<const char *> *pSymbols, FuncOnSymbol funcOnSymbol);

// updSymbol - update symbol
bool updSymbol(const char *host, const char *token,
               tradingpb::SymbolInfo &si);

CR2END

#endif  // __TRADINGCORE2_TRDB2_CLIENT_H__