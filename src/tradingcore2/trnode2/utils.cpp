#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <tradingcore2/autorun.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/spinlock.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trnode2/serv.h>
#include <tradingcore2/trnode2/utils.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <string>

CR2BEGIN

// findAssetData - find PNLAssetData
::tradingpb::PNLAssetData* findAssetData(::tradingpb::PNLData* pPNLData,
                                         const char* market,
                                         const char* symbol) {
  assert(pPNLData != NULL);
  assert(market != NULL);
  assert(symbol != NULL);

  for (auto i = 0; i < pPNLData->assets_size(); ++i) {
    auto curasset = pPNLData->mutable_assets(i);
    auto a = curasset->asset();
    if (a.market() == market && a.code() == symbol) {
      return curasset;
    }
  }

  return NULL;
}

// findAssetDataEx - find PNLAssetData
//      if can't find it, it will new a PNLAssetData
::tradingpb::PNLAssetData* findAssetDataEx(::tradingpb::PNLData* pPNLData,
                                           const char* market,
                                           const char* symbol) {
  auto pData = findAssetData(pPNLData, market, symbol);
  if (pData == NULL) {
    pData = pPNLData->add_assets();
    auto asset = pData->mutable_asset();
    asset->set_market(market);
    asset->set_code(symbol);
  }

  return pData;
}

// insTimestamp - insert a timestamp
void insTimestamp(::tradingpb::PNLAssetData* pAssetData, time_t ts) {
  assert(pAssetData != NULL);
}

CR2END