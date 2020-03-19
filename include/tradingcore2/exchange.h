#ifndef __TRADINGCORE2_EXCHANGE_H__
#define __TRADINGCORE2_EXCHANGE_H__

#include <tradingcore2/basedef.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class Exchange {
 public:
  // FuncOnTimeStamp - for Exchange::forEachTimeStamp
  typedef std::function<void(const Exchange&, TimeStamp)> FuncOnTimeStamp;
  // FuncOnAssetsData - for Exchange::forEachAssetsData
  typedef std::function<void(const char* assetsName, TimeStamp ts, Money price,
                             Volume volume)>
      FuncOnAssetsData;

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

  virtual void forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                TimeStamp tsStart, TimeStamp tsEnd) const = 0;

  virtual void forEachAssetsData(const char* assetsName,
                                 Exchange::FuncOnAssetsData func,
                                 TimeStamp tsStart, TimeStamp tsEnd) const = 0;

  virtual TimeStamp getLastTimeStamp() const = 0;

  virtual int getTradingDays4Year() const = 0;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_H__