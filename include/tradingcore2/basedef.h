#ifndef __TRADINGCORE2_BASEDEF_H__
#define __TRADINGCORE2_BASEDEF_H__

#include <time.h>

#define CR2BEGIN namespace tr2 {
#define CR2END }

CR2BEGIN

typedef time_t TimeStamp;

typedef double Money;
typedef double Volume;

const Money ZEROMONEY = 0.0;
const Volume ZEROVOLUME = 0.0;
// typedef long int Volume;

class Wallet;
class Exchange;

CR2END

#endif  // __TRADINGCORE2_BASEDEF_H__