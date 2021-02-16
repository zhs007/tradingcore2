
#include <dirent.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

const char* CNFundTypeName = "cnfund";

const CNFundValueNode* CNFundValue::getNode(TimeStamp ts) const {
  for (auto it = this->data.begin(); it != this->data.end(); ++it) {
    if (it->ts >= ts) {
      return &(*it);
    }
  }

  return NULL;
}

bool CNFundExchange::init(const Config& cfg) {
  CNFundExchange* cnfund = this;
  auto onfile = [&cnfund](const char* dir, const char* fn) {
    cnfund->loadFundValue(joinPath(dir, fn).c_str());
  };

  tr2::foreachPathWithExt(cfg.cnfundPath.c_str(), ".csv", onfile);

  this->rebuildTimeStampList(NULL);

  return true;
}

const char* CNFundExchange::getTypeName() const { return CNFundTypeName; }

// loadDat - [tsStart, tsEnd]
void CNFundExchange::loadData(const char* assetName, TimeStamp tsStart,
                              TimeStamp tsEnd, int offset) {}

void CNFundExchange::setFundValue(const char* assetsName, CNFundValue* fv) {
  this->releaseFundValue(assetsName);

  Pair p;
  p.first = assetsName;
  p.second = fv;

  auto itret = this->m_map.insert(p);
  assert(itret.second);
}

void CNFundExchange::releaseFundValue(const char* assetsName) {
  auto it = this->m_map.find(assetsName);
  if (it != this->m_map.end()) {
    delete it->second;

    this->m_map.erase(it);
  }
}

void CNFundExchange::loadFundValue(const char* fn) {
  CSVLoader csv;
  CNFundValue* fv = new CNFundValue();

  csv.load(fn);

  fv->code = csv.getData(0, 0);

  for (int y = 0; y < csv.getHeight(); ++y) {
    CNFundValueNode node;

    node.value = strtoll(csv.getData(2, y), NULL, 10) / 10000.0;
    node.totalValue = strtoll(csv.getData(3, y), NULL, 10) / 10000.0;
    node.ts = str2timestampUTC(csv.getData(1, y), "%Y%m%d");

    fv->data.push_back(node);
  }

  this->setFundValue(csv.getData(0, 0), fv);
}

const CNFundValue* CNFundExchange::getFundValue(const char* assetsName) const {
  auto it = m_map.find(assetsName);
  if (it == m_map.end()) {
    return NULL;
  }

  return it->second;
}

bool CNFundExchange::calculateVolume(const char* assetsName, TimeStamp ts,
                                     Money money, Volume& volume, Money& price,
                                     Money& fee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(money > ZEROMONEY);

  auto fv = this->getFundValue(assetsName);
  if (fv == NULL) {
    return false;
  }

  auto n = fv->getNode(ts);
  if (n == NULL) {
    return false;
  }

  volume = money / n->value;
  price = n->value;
  fee = ZEROMONEY;

  return true;
}

bool CNFundExchange::calculatePrice(const char* assetsName, TimeStamp ts,
                                    Volume volume, Money& money, Money& price,
                                    Money& fee) {
  assert(assetsName != NULL);
  assert(ts > 0);
  assert(volume > ZEROVOLUME);

  auto fv = this->getFundValue(assetsName);
  if (fv == NULL) {
    return false;
  }

  auto n = fv->getNode(ts);
  if (n == NULL) {
    return false;
  }

  money = volume * n->value;
  price = n->value;
  fee = ZEROMONEY;

  return true;
}

bool CNFundExchange::getDataWithTimestamp(const char* assetsName, TimeStamp ts,
                                          CandleData& data) const {
  auto fv = this->getFundValue(assetsName);
  assert(fv != NULL);

  auto n = fv->getNode(ts);
  if (n == NULL) {
    return false;
  }

  data.ts = ts;
  data.open = data.close = data.high = data.low = n->totalValue;
  data.volume = ZEROVOLUME;

  return true;
}

bool CNFundExchange::getData(const char* assetsName, int index,
                             CandleData& data) {
  assert(index >= 0);

  auto fv = this->getFundValue(assetsName);
  assert(fv != NULL);

  if (index < fv->data.size()) {
    data.ts = fv->data[index].ts;
    data.open = data.close = data.high = data.low = fv->data[index].totalValue;
    data.volume = ZEROVOLUME;

    return true;
  }

  return false;
}

int CNFundExchange::getDataLength(const char* assetsName) {
  auto fv = this->getFundValue(assetsName);
  if (fv == NULL) {
    return 0;
  }

  return fv->data.size();
}

void CNFundExchange::forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                      TimeStamp tsStart,
                                      TimeStamp tsEnd) const {
  if (tsEnd <= 0) {
    tsEnd = this->m_lstTimeStamp.back();
  }

  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    if (*it >= tsEnd) {
      break;
    }

    if (*it >= tsStart) {
      func(*this, *it, std::distance(this->m_lstTimeStamp.begin(), it));
    }
  }
}

void CNFundExchange::forEachAssetsData(const char* assetsName,
                                       Exchange::FuncOnAssetsData func,
                                       TimeStamp tsStart,
                                       TimeStamp tsEnd) const {
  auto fv = this->getFundValue(assetsName);
  assert(fv != NULL);

  if (tsStart == tsEnd) {
    for (auto it = fv->data.begin(); it != fv->data.end(); ++it) {
      if (it->ts > tsEnd) {
        break;
      }

      if (it->ts >= tsStart) {
        CandleData cd(it->ts, it->totalValue);

        func(assetsName, &cd);
      }
    }

    return;
  }

  for (auto it = fv->data.begin(); it != fv->data.end(); ++it) {
    if (it->ts >= tsEnd) {
      break;
    }

    if (it->ts >= tsStart) {
      CandleData cd(it->ts, it->totalValue);

      func(assetsName, &cd);
    }
  }
}

void CNFundExchange::rebuildTimeStampList(const char* assetsName) {
  this->m_lstTimeStamp.clear();

  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    for (auto ait = it->second->data.begin(); ait != it->second->data.end();
         ++ait) {
      this->insertTimeStamp(ait->ts);
    }
  }
}

void CNFundExchange::insertTimeStamp(TimeStamp ts) {
  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    if (ts == *it) {
      return;
    }

    if (ts < *it) {
      this->m_lstTimeStamp.insert(it, ts);

      return;
    }
  }

  this->m_lstTimeStamp.push_back(ts);
}

TimeStamp CNFundExchange::getLastTimeStamp() const {
  assert(!this->m_lstTimeStamp.empty());

  return this->m_lstTimeStamp[this->m_lstTimeStamp.size() - 1];
}

TimeStamp CNFundExchange::getFirstTimeStamp() const {
  assert(!this->m_lstTimeStamp.empty());

  return this->m_lstTimeStamp[0];
}

void CNFundExchange::release() {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    delete it->second;
  }

  this->m_map.clear();
}

bool CNFundExchange::isValidTs(TimeStamp ts) const {
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

Exchange* CNFundExchange::newExchange(const Config& cfg) {
  auto exchange = new CNFundExchange();

  exchange->init(cfg);

  return exchange;
}

void CNFundExchange::regExchange() {
  ExchangeMgr::getSingleton()->regNewExchange(CNFundTypeName,
                                              CNFundExchange::newExchange);
}

CR2END