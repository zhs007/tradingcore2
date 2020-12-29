#ifndef __TRADINGCORE2_EXCHANGE_BITMEX_H__
#define __TRADINGCORE2_EXCHANGE_BITMEX_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/exchange/trdb2.h>
#include <tradingcore2/trdb2/datamgr.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

class BitMexExchange final : public TrDB2Exchange {
 public:
  static Exchange* newExchange(const Config& cfg);

  static void regExchange();

 private:
  BitMexExchange(const char* host, const char* token)
      : TrDB2Exchange(host, token) {}
  virtual ~BitMexExchange() { this->release(); }

 public:
  virtual const char* getTypeName() const override;

  virtual int getTradingDays4Year() const override;

  virtual float getRiskFreeInterestRate() const override { return 0.03; }

  virtual const char* getMarketName() const override { return "bitmex"; }

  virtual bool isValidTs(TimeStamp ts) const override;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGE_BITMEX_H__