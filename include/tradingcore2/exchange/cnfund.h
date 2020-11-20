#ifndef __TRADINGCORE2_EXCHANGE_CNFUND_H__
#define __TRADINGCORE2_EXCHANGE_CNFUND_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/exchange.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

struct CNFundValueNode {
  Money value;
  Money totalValue;
  TimeStamp ts;
};

struct CNFundValue {
  std::string code;
  std::vector<CNFundValueNode> data;

  const CNFundValueNode* getNode(TimeStamp ts) const;
};

static const char* CNFundTypeName = "cnfund";

Exchange* newCNFund(const Config& cfg);

class CNFundExchange final : public Exchange {
  friend Exchange* newCNFund(const Config& cfg);

 public:
  typedef std::map<std::string, CNFundValue*> Map;
  typedef std::pair<std::string, CNFundValue*> Pair;
  typedef std::vector<TimeStamp> TimeStampList;

 private:
  CNFundExchange() {}
  virtual ~CNFundExchange() { this->release(); }

 public:
  virtual bool init(const Config& cfg) override;

  virtual const char* getTypeName() const override;

  // loadDat - [tsStart, tsEnd]
  virtual void loadData(const char* assetName, TimeStamp tsStart,
                        TimeStamp tsEnd) override;

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

  virtual int getTradingDays4Year() const override { return 250; }

  virtual float getRiskFreeInterestRate() const override { return 0.03; }

  virtual const char* getMarketName() const override { return "cnfund"; }

  virtual void rebuildTimeStampList() override;

 public:
  const CNFundValue* getFundValue(const char* assetsName) const;

  void loadFundValue(const char* fn);

  void releaseFundValue(const char* assetsName);

  void setFundValue(const char* assetsName, CNFundValue* fv);

  void insertTimeStamp(TimeStamp ts);

  void release();

 protected:
  Map m_map;
  TimeStampList m_lstTimeStamp;
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_CNFUND_H__