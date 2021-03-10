#ifndef __TRADINGCORE2_EXCHANGE_TRDB2_H__
#define __TRADINGCORE2_EXCHANGE_TRDB2_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/trdb2/datamgr.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

class TrDB2Exchange : public Exchange {
 public:
  typedef std::vector<TimeStamp> TimeStampList;

 protected:
  TrDB2Exchange(const char* host, const char* token) : m_mgrData(host, token) {}

 public:
  virtual bool init(const Config& cfg) override;

  // loadDat - [tsStart, tsEnd]
  virtual void loadData(const char* assetName, TimeStamp tsStart,
                        TimeStamp tsEnd, int offset) override;

 public:
  virtual bool calculateVolume(const char* assetsName, TimeStamp ts,
                               Money money, Volume& volume, Money& price,
                               Money& fee) override;

  virtual bool calculatePrice(const char* assetsName, TimeStamp ts,
                              Volume volume, Money& money, Money& price,
                              Money& fee) override;

  virtual bool getDataWithTimestamp(const char* assetsName, TimeStamp ts,
                                    CandleData& data) const override;

  virtual bool getData(const char* assetsName, int index,
                       CandleData& data) override;

  virtual int getDataLength(const char* assetsName) override;

  virtual void forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                TimeStamp tsStart,
                                TimeStamp tsEnd) const override;

  virtual void forEachAssetsData(const char* assetsName,
                                 Exchange::FuncOnAssetsData func,
                                 TimeStamp tsStart,
                                 TimeStamp tsEnd) const override;

  virtual TimeStamp getFirstTimeStamp() const override;

  virtual TimeStamp getLastTimeStamp() const override;

  virtual void rebuildTimeStampList(const char* assetsName) override;

  virtual void mergeCandles(const tradingpb::Candles* candles) override;

 public:
  void foreachCandlesTimeStamp(const tradingpb::Candles* candles);

  void insertTimeStamp(TimeStamp ts);

  void release();

 protected:
  TrDB2DataMgr m_mgrData;
  TimeStampList m_lstTimeStamp;
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_TRDB2_H__