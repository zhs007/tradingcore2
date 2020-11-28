#include <tradingcore2/allstrategy.h>
#include <tradingcore2/strategy/aip.h>
#include <tradingcore2/strategy/buyandhold.h>
#include <tradingcore2/strategy/normal.h>

CR2BEGIN

void regAllStrategy() {
  StrategyAIP::regStrategy();
  StrategyBAH::regStrategy();
  StrategyNormal::regStrategy();
}

CR2END