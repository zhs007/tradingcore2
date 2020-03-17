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

class CNFundExchange : public Exchange {
 public:
  typedef std::map<std::string, CNFundValue*> Map;
  typedef std::pair<std::string, CNFundValue*> Pair;
  typedef std::vector<TimeStamp> TimeStampList;

 public:
  CNFundExchange() {}
  virtual ~CNFundExchange() {}

 public:
  virtual bool calculateVolume(const char* assetsName, TimeStamp ts,
                               Money money, Volume& volume, Money& price,
                               Money& fee);

  virtual bool calculatePrice(const char* assetsName, TimeStamp ts,
                              Volume volume, Money& money, Money& price,
                              Money& fee);

  virtual bool getDataWithTimestamp(const char* assetsName, TimeStamp ts,
                                    Money& price, Volume& volume);

  virtual bool getData(const char* assetsName, int index, TimeStamp& ts,
                       Money& price, Volume& volume);

  virtual int getDataLength(const char* assetsName);

  virtual void forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                TimeStamp tsStart, TimeStamp tsEnd) const;

  virtual void forEachAssetsData(const char* assetsName,
                                 Exchange::FuncOnAssetsData func,
                                 TimeStamp tsStart, TimeStamp tsEnd) const;

  virtual TimeStamp getLastTimeStamp() const;

 public:
  const CNFundValue* getFundValue(const char* assetsName) const;

  void loadFundValue(const char* fn);

  void releaseFundValue(const char* assetsName);

  void setFundValue(const char* assetsName, CNFundValue* fv);

  void buildTimeStampList();

  void insertTimeStamp(TimeStamp ts);

 protected:
  Map m_map;
  TimeStampList m_lstTimeStamp;
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_CNFUND_H__