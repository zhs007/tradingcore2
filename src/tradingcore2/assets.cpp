
#include <tradingcore2/assets.h>
#include <tradingcore2/exchange.h>

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

CR2END