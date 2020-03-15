#include <assert.h>
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/rsi.h>
#include <tradingcore2/utils.h>
#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorRSI::pushData(TimeStamp ts, IndicatorDataValue u,
                            IndicatorDataValue d, IndicatorDataValue smmau,
                            IndicatorDataValue smmad, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.u = u;
  n.d = d;
  n.smmau = smmau;
  n.smmad = smmad;
  n.val.value = val;

  m_lst.push_back(n);
}

bool IndicatorRSI::build(Exchange& exchange, const char* assetsName, int start,
                         int length) {
  assert(assetsName != NULL);
  assert(start >= 0);
  assert(length > 0);

  this->release();

  int totalLength = exchange.getDataLength(assetsName);
  if (start >= totalLength) {
    return false;
  }

  if (start + length >= totalLength) {
    length = totalLength - start;
  }

  if (length <= 0) {
    return false;
  }

  m_iStart = start;

  Money price;
  Volume volume;
  TimeStamp ts;
  auto isok = exchange.getData(assetsName, start, ts, price, volume);
  assert(isok);

  Money preprice = price;
  IndicatorDataValue presmmau = 0;
  IndicatorDataValue presmmad = 0;

  this->pushData(ts, 0.0, 0.0, 0.0, 0.0, 100);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, ts, price, volume);
    assert(isok);

    IndicatorDataValue u, d, smmau, smmad;

    if (price > preprice) {
      u = price - preprice;
      d = 0;
    } else {
      u = 0;
      d = preprice - price;
    }

    smmau = (presmmau * (this->m_avgtimes - 1) + u) / this->m_avgtimes;
    smmad = (presmmad * (this->m_avgtimes - 1) + d) / this->m_avgtimes;

    IndicatorDataValue rsi = 100;
    if (smmad != 0) {
      rsi = 100 - 100 / (1 + smmau / smmad);
    }

    this->pushData(ts, u, d, smmau, smmad, rsi);

    preprice = price;
    presmmau = smmau;
    presmmad = smmad;
  }

  return true;
}

CR2END