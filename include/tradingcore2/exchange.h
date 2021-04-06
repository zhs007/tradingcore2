#ifndef __TRADINGCORE2_EXCHANGE_H__
#define __TRADINGCORE2_EXCHANGE_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/config.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/wallet.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

class Exchange {
 public:
  // FuncOnTimeStamp - for Exchange::forEachTimeStamp
  typedef std::function<void(const Exchange&, TimeStamp, int)> FuncOnTimeStamp;
  // FuncOnAssetsData - for Exchange::forEachAssetsData
  typedef std::function<void(const char* assetsName, const CandleData* pData)>
      FuncOnAssetsData;

 public:
  Exchange() {}
  virtual ~Exchange() {}

 public:
  virtual bool init(const Config& cfg) = 0;

  virtual const char* getTypeName() const = 0;

  // loadDat - [tsStart, tsEnd]
  virtual void loadData(const char* assetName, TimeStamp tsStart,
                        TimeStamp tsEnd, int offset) = 0;

 public:
  virtual bool calculateVolume(const char* assetsName, TimeStamp ts,
                               Money money, Volume& volume, Money& price,
                               Money& fee, FuncCalcFee calcFee) = 0;

  virtual bool calculateVolumeWithLimitPrice(const char* assetsName,
                                             TimeStamp ts, Money money,
                                             Volume& volume, Money& price,
                                             Money& fee, Money limitPrice,
                                             FuncCalcFee calcFee) = 0;

  virtual bool calculatePrice(const char* assetsName, TimeStamp ts,
                              Volume volume, Money& money, Money& price,
                              Money& fee) = 0;

  virtual bool getDataWithTimestamp(const char* assetsName, TimeStamp ts,
                                    CandleData& data) const = 0;

  virtual bool getData(const char* assetsName, int index, CandleData& data) = 0;

  virtual int getDataLength(const char* assetsName) = 0;

  virtual void forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                TimeStamp tsStart, TimeStamp tsEnd) const = 0;

  virtual void forEachAssetsData(const char* assetsName,
                                 Exchange::FuncOnAssetsData func,
                                 TimeStamp tsStart, TimeStamp tsEnd) const = 0;

  virtual TimeStamp getLastTimeStamp() const = 0;

  virtual TimeStamp getFirstTimeStamp() const = 0;

  virtual int getTradingDays4Year() const = 0;

  virtual float getRiskFreeInterestRate() const = 0;

  virtual const char* getMarketName() const = 0;

  virtual void rebuildTimeStampList(const char* assetsName) = 0;

  virtual bool isValidTs(TimeStamp ts) const = 0;

  virtual void mergeCandles(const tradingpb::Candles* candles) = 0;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_H__