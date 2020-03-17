#include <assert.h>
#include <stdlib.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

const CNFundValueNode* CNFundValue::getNode(TimeStamp ts) const {
  for (auto it = this->data.begin(); it != this->data.end(); ++it) {
    if (it->ts >= ts) {
      return &(*it);
    }
  }

  return NULL;
}

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
                                          Money& price, Volume& volume) {
  auto fv = this->getFundValue(assetsName);
  assert(fv != NULL);

  auto n = fv->getNode(ts);
  if (n == NULL) {
    return false;
  }

  price = n->totalValue;
  volume = ZEROVOLUME;

  return true;
}

bool CNFundExchange::getData(const char* assetsName, int index, TimeStamp& ts,
                             Money& price, Volume& volume) {
  assert(index >= 0);

  auto fv = this->getFundValue(assetsName);
  assert(fv != NULL);

  if (index < fv->data.size()) {
    price = fv->data[index].totalValue;
    volume = ZEROVOLUME;
    ts = fv->data[index].ts;

    return true;
  }

  return false;
}

int CNFundExchange::getDataLength(const char* assetsName) {
  auto fv = this->getFundValue(assetsName);
  assert(fv != NULL);

  return fv->data.size();
}

void CNFundExchange::forEachTimeStamp(Exchange::FuncOnTimeStamp func,
                                      TimeStamp tsStart,
                                      TimeStamp tsEnd) const {
  for (auto it = this->m_lstTimeStamp.begin(); it != this->m_lstTimeStamp.end();
       ++it) {
    if (*it >= tsEnd) {
      break;
    }

    if (*it >= tsStart) {
      func(*this, *it);
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
        func(assetsName, it->ts, it->value, ZEROVOLUME);
      }
    }

    return;
  }

  for (auto it = fv->data.begin(); it != fv->data.end(); ++it) {
    if (it->ts >= tsEnd) {
      break;
    }

    if (it->ts >= tsStart) {
      func(assetsName, it->ts, it->value, ZEROVOLUME);
    }
  }
}

void CNFundExchange::buildTimeStampList() {
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

CR2END