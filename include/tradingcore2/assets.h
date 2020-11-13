#ifndef __TRADINGCORE2_ASSETS_H__
#define __TRADINGCORE2_ASSETS_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/candle.h>
#include <tradingcore2/trade.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

CR2BEGIN

struct AssetsBlock {
  Volume volume;
  Money inPrice;
  Money feesPaid;
  TimeStamp ts;
};

struct Assets {
  std::string name;
  Volume volume;
  Money inPrice;
  Money feesPaid;
  std::vector<AssetsBlock> blocks;

  Assets() : volume(ZEROVOLUME), inPrice(ZEROMONEY), feesPaid(ZEROMONEY) {}
};

class AssetsMap {
 public:
  typedef std::map<std::string, Assets> Map;
  typedef std::pair<std::string, Assets> Pair;

 public:
  AssetsMap() {}
  ~AssetsMap() {}

 public:
  const Assets* getAssets(const char* assetsName) const;

  void buyAssets(const char* assetsName, TimeStamp ts, Money price,
                 Volume volume, Money fee);

  void sellAssets(const char* assetsName, TimeStamp ts, Money price,
                  Volume volume, Money fee);

  void makePNL(PNL& pnl, const Exchange& exchange, Money invest,
               Money handMoney, TimeStamp tsStart, TimeStamp tsEnd);

  void makePNL2(PNL2& pnl2, const Exchange& exchange, Money invest,
               Money handMoney, TimeStamp tsStart, TimeStamp tsEnd);

 protected:
  Assets* _getAssets(const char* assetsName);

  void onAssetsDataForPNL(PNL* pPNL, const char* assetsName,
                          const CandleData* pData, Volume assetsVolume);

 protected:
  Map m_map;
};

CR2END

#endif  // __TRADINGCORE2_ASSETS_H__