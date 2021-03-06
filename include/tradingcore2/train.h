#ifndef __TRADINGCORE2_TRAIN_H__
#define __TRADINGCORE2_TRAIN_H__

#include <tradingcore2/basedef.h>

#include <functional>
#include <string>

CR2BEGIN

void saveTrainResult(const char* fn, TrainResultList& lst);

bool calcIndicatorRange(Exchange& exchange, const char* assetsName,
                        const char* indicatorName, int avgtimes,
                        IndicatorDataValue& minval, IndicatorDataValue& maxval);

// 统计计算次数
int calcSingleIndicatorTimes(IndicatorDataValue minval,
                             IndicatorDataValue maxval, IndicatorDataValue off0,
                             IndicatorDataValue off1, IndicatorDataValue off2,
                             IndicatorDataValue maxoff2);

// trainSingleIndicator - 单独一个indicator的策略，不考虑方向，只考虑值
//                        off0 是买卖起始点的偏移
//                        off1 是买和卖之间的偏移
//                        off2 是买或卖范围的偏移
//                        maxoff2 是买或卖的范围偏移最大值
//                        minValidReturn 表示回报大于这个值才会输出数据
bool trainSingleIndicator(Exchange& exchange, const char* assetsName,
                          const char* indicatorName, const char* outputPath,
                          Money invest, int avgtimes, IndicatorDataValue off0,
                          IndicatorDataValue off1, IndicatorDataValue off2,
                          IndicatorDataValue maxoff2, float minValidReturn);

// 统计计算次数
int calcSingleIndicatorTimes2(IndicatorDataValue minval,
                              IndicatorDataValue maxval,
                              IndicatorDataValue off0, IndicatorDataValue off1,
                              IndicatorDataValue off2,
                              IndicatorDataValue maxoff2);

// trainSingleIndicator2 - 单独一个indicator的策略，考虑方向
bool trainSingleIndicator2(Exchange& exchange, const char* assetsName,
                           const char* indicatorName, const char* outputPath,
                           Money invest, int avgtimes, IndicatorDataValue off0,
                           IndicatorDataValue off1, IndicatorDataValue off2,
                           IndicatorDataValue maxoff2, float minValidReturn);

// FuncOnTrainEnd - for _trainSingleIndicator2Ex
typedef std::function<void(const Wallet& wallet, TrainResult& tr)>
    FuncOnTrainEnd;

// _trainSingleIndicator2Ex -
// 单独一个indicator的策略，考虑方向，一次只算若干次数，用于server分布式运算
bool _trainSingleIndicator2Ex(TrainResultList& lst, Exchange& exchange,
                              const char* assetsName, const char* indicatorName,
                              const char* outputPath, Money invest,
                              int avgtimes, IndicatorDataValue off0,
                              IndicatorDataValue off1, IndicatorDataValue off2,
                              IndicatorDataValue maxoff2, float minValidReturn,
                              IndicatorDataValue minval,
                              IndicatorDataValue maxval, IndicatorDataValue cv0,
                              IndicatorDataValue cv0off, FuncOnTrainEnd onend);

// trainSingleIndicator2Ex -
// 单独一个indicator的策略，考虑方向，调用 _trainSingleIndicator2Ex
bool trainSingleIndicator2Ex(Exchange& exchange, const char* assetsName,
                             const char* indicatorName, const char* outputPath,
                             Money invest, int avgtimes,
                             IndicatorDataValue off0, IndicatorDataValue off1,
                             IndicatorDataValue off2,
                             IndicatorDataValue maxoff2, float minValidReturn);

CR2END

#endif  // __TRADINGCORE2_TRAIN_H__