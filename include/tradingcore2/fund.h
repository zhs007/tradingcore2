#ifndef __TRADINGCORE2_FUND_H__
#define __TRADINGCORE2_FUND_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

// analysisFund - analysis fund
bool analysisFund(PNL& pnl, const Exchange& exchange,
                  const tradingpb::SymbolInfo& si,
                  const tradingpb::Candles& candles);

CR2END

#endif  // __TRADINGCORE2_FUND_H__