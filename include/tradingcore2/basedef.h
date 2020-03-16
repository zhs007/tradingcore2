#ifndef __TRADINGCORE2_BASEDEF_H__
#define __TRADINGCORE2_BASEDEF_H__

#include <time.h>

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

CR2END

#endif  // __TRADINGCORE2_BASEDEF_H__