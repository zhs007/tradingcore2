#ifndef __TRADINGCORE2_CLIENT_TRAIN2P_H__
#define __TRADINGCORE2_CLIENT_TRAIN2P_H__

#include <tradingcore2/basedef.h>

CR2BEGIN

// startTrainSingleIndicator2ExPool -
// 单独一个indicator的策略，考虑方向，grpc调用
bool startTrainSingleIndicator2ExPool(
    const Config& cfg, Exchange& exchange, const char* assetsName,
    const char* indicatorName, const char* outputPath, Money invest,
    int avgtimes, IndicatorDataValue off0, IndicatorDataValue off1,
    IndicatorDataValue off2, IndicatorDataValue maxoff2, float minValidReturn);

CR2END

#endif  // __TRADINGCORE2_CLIENT_TRAIN2P_H__