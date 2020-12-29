
#include <dirent.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/bitmex.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/trdb2/utils.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

const char* BitMexTypeName = "bitmex";

const char* BitMexExchange::getTypeName() const { return BitMexTypeName; }

int BitMexExchange::getTradingDays4Year() const {
  return this->m_mgrData.calcAverageTradingDays4Year();
}

bool BitMexExchange::isValidTs(TimeStamp ts) const {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    tm curtm;
    timestamp2timeUTC(*it, curtm);

    if (curtm.tm_year == ctm.tm_year && curtm.tm_yday == ctm.tm_yday) {
      return true;
    } else if (curtm.tm_year >= ctm.tm_year && curtm.tm_yday > ctm.tm_yday) {
      return false;
    }
  }

  return false;
}

Exchange* BitMexExchange::newExchange(const Config& cfg) {
  auto exchange =
      new BitMexExchange(cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str());

  exchange->init(cfg);

  return exchange;
}

void BitMexExchange::regExchange() {
  ExchangeMgr::getSingleton()->regNewExchange(BitMexTypeName,
                                              BitMexExchange::newExchange);
}

CR2END