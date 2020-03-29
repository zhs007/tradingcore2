#ifndef __TRADINGCORE2_TRAIN_H__
#define __TRADINGCORE2_TRAIN_H__

#include <tradingcore2/basedef.h>

#include <string>

CR2BEGIN

void saveTrainResult(const char* fn, TrainResultList& lst);

bool calcIndicatorRange(Exchange& exchange, const char* assetsName,
                        const char* indicatorName, int avgtimes,
                        IndicatorDataValue& minval, IndicatorDataValue& maxval);

bool trainSingleIndicator(Exchange& exchange, const char* assetsName,
                          const char* indicatorName, const char* outputPath,
                          Money invest, int avgtimes, IndicatorDataValue off0,
                          IndicatorDataValue off1);

CR2END

#endif  // __TRADINGCORE2_TRAIN_H__