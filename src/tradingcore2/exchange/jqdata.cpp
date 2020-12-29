
#include <dirent.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/jqdata.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/trdb2/utils.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

const char* JQDataTypeName = "jqdata";

const char* JQDataExchange::getTypeName() const { return JQDataTypeName; }

int JQDataExchange::getTradingDays4Year() const {
  return this->m_mgrData.calcAverageTradingDays4Year();
}

bool JQDataExchange::isValidTs(TimeStamp ts) const {
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

Exchange* JQDataExchange::newExchange(const Config& cfg) {
  auto exchange =
      new JQDataExchange(cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str());

  exchange->init(cfg);

  return exchange;
}

void JQDataExchange::regExchange() {
  ExchangeMgr::getSingleton()->regNewExchange(JQDataTypeName,
                                              JQDataExchange::newExchange);
}

CR2END