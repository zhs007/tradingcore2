
#include <tradingcore2/assets.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/pnl2.h>

#include <functional>

CR2BEGIN

const Assets* AssetsMap::getAssets(const char* assetsName) const {
  auto it = this->m_map.find(assetsName);
  if (it != this->m_map.end()) {
    return &(it->second);
  }

  return NULL;
}

void AssetsMap::buyAssets(const char* assetsName, TimeStamp ts, Money price,
                          Volume volume, Money fee) {
  auto assets = this->_getAssets(assetsName);
  assert(assets != NULL);

  assets->inPrice = (assets->inPrice * assets->volume + volume * price) /
                    (assets->volume + volume);
  assets->volume += volume;
  assets->feesPaid += fee;

  AssetsBlock ab;

  ab.volume = volume;
  ab.inPrice = price;
  ab.feesPaid = fee;
  ab.ts = ts;

  assets->blocks.push_back(ab);
}

void AssetsMap::sellAssets(const char* assetsName, TimeStamp ts, Money price,
                           Volume volume, Money fee) {
  auto assets = this->_getAssets(assetsName);
  assert(assets != NULL);
  assert(assets->volume >= volume);

  assets->volume -= volume;
  assets->feesPaid += fee;

  if (assets->volume == ZEROVOLUME) {
    assets->inPrice = ZEROMONEY;

    assets->blocks.clear();

    return;
  }

  for (auto it = assets->blocks.begin(); it != assets->blocks.end();) {
    if (it->volume > volume) {
      it->volume -= volume;

      break;
    }

    volume -= it->volume;
    it = assets->blocks.erase(it);

    if (volume <= 0) {
      break;
    }
  }
}

Assets* AssetsMap::_getAssets(const char* assetsName) {
  auto it = this->m_map.find(assetsName);
  if (it != this->m_map.end()) {
    return &(it->second);
  }

  Pair p;
  p.first = assetsName;
  p.second.name = assetsName;

  auto ret = this->m_map.insert(p);
  assert(ret.second);

  it = this->m_map.find(assetsName);
  assert(it != this->m_map.end());

  return &(it->second);
}

void AssetsMap::makePNL(PNL& pnl, const Exchange& exchange, Money invest,
                        Money handMoney, TimeStamp tsStart, TimeStamp tsEnd) {
  pnl.initInvest(exchange, invest, handMoney, tsStart, tsEnd);

  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    auto f = std::bind(&AssetsMap::onAssetsDataForPNL, this, &pnl,
                       std::placeholders::_1, std::placeholders::_2,
                       it->second.volume);

    exchange.forEachAssetsData(it->second.name.c_str(), f, tsStart, tsEnd);
  }
}

void AssetsMap::onAssetsDataForPNL(PNL* pPNL, const char* assetsName,
                                   const CandleData* pData,
                                   Volume assetsVolume) {
  assert(pData != NULL);

  pPNL->chgData(pData->ts, 0, pData->close * assetsVolume);
}

void AssetsMap::makePNL2(PNL2& pnl2, const Exchange& exchange, Money invest,
                         Money handMoney, TimeStamp tsStart, TimeStamp tsEnd) {
  // pnl.initInvest(exchange, invest, handMoney, tsStart, tsEnd);

  // for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
  //   auto f = std::bind(&AssetsMap::onAssetsDataForPNL, this, &pnl,
  //                      std::placeholders::_1, std::placeholders::_2,
  //                      it->second.volume);

  //   exchange.forEachAssetsData(it->second.name.c_str(), f, tsStart, tsEnd);
  // }
}

CR2END