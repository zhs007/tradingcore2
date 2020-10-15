#ifndef __TRADINGCORE2_FUND_H__
#define __TRADINGCORE2_FUND_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/proto/tradingdb2.grpc.pb.h>

#include <functional>

CR2BEGIN

// analysisFund - analysis fund
bool analysisFund(PNL& pnl, const Exchange& exchange,
                  const tradingdb2pb::Candles& candles);

CR2END

#endif  // __TRADINGCORE2_FUND_H__