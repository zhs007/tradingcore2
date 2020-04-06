#ifndef __TRADINGCORE2_BASEDEF_H__
#define __TRADINGCORE2_BASEDEF_H__

#include <assert.h>
#include <time.h>

#include <string>
#include <vector>

#define CR2BEGIN namespace tr2 {
#define CR2END }

CR2BEGIN

typedef time_t TimeStamp;

typedef double Money;
typedef double Volume;
typedef double IndicatorDataValue;

const Money ZEROMONEY = 0.0;
const Volume ZEROVOLUME = 0.0;
// typedef long int Volume;

class Wallet;
class Exchange;
class Indicator;
class PNL;

struct WalletHistoryNode;
struct Config;

struct TrainResult {
  std::string name;
  float totalReturn;
  float maxDrawDown;
  float sharpe;
  float annualizedReturns;
  float annualizedVolatility;
  int tradingNums;
  int stoplossNums;
  float winRate;
};

typedef std::vector<TrainResult> TrainResultList;

CR2END

#endif  // __TRADINGCORE2_BASEDEF_H__