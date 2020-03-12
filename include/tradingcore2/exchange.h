#ifndef __TRADINGCORE2_EXCHANGE_H__
#define __TRADINGCORE2_EXCHANGE_H__

#include <tradingcore2/basedef.h>
#include <map>
#include <string>

CR2BEGIN

class Exchange {
 public:
  Exchange() {}
  virtual ~Exchange() {}

 public:
  virtual bool calculateVolume(const char* assetsName, TimeStamp ts, Money money,
                               Volume& volume, Money& price, Money& fee) = 0;

  virtual bool calculatePrice(const char* assetsName, TimeStamp ts, Volume volume,
                              Money& money, Money& price, Money& fee) = 0;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_H__