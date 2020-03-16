#ifndef __TRADINGCORE2_EXCHANGE_H__
#define __TRADINGCORE2_EXCHANGE_H__

#include <tradingcore2/basedef.h>
#include <map>
#include <string>

CR2BEGIN

// FuncOnTimer - for Exchange::forEachTimeStamp
typedef std::function<void(const Exchange&, TimeStamp)> FuncOnTimer;

class Exchange {
 public:
  Exchange() {}
  virtual ~Exchange() {}

 public:
  virtual bool calculateVolume(const char* assetsName, TimeStamp ts,
                               Money money, Volume& volume, Money& price,
                               Money& fee) = 0;

  virtual bool calculatePrice(const char* assetsName, TimeStamp ts,
                              Volume volume, Money& money, Money& price,
                              Money& fee) = 0;

  virtual bool getDataWithTimestamp(const char* assetsName, TimeStamp ts,
                                    Money& price, Volume& volume) = 0;

  virtual bool getData(const char* assetsName, int index, TimeStamp& ts,
                       Money& price, Volume& volume) = 0;

  virtual int getDataLength(const char* assetsName) = 0;

  virtual void forEachTimeStamp(FuncOnTimer func) const = 0;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_H__