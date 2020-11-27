#include <tradingcore2/allstrategy.h>
#include <tradingcore2/strategy/aip.h>
#include <tradingcore2/strategy/buyandhold.h>

CR2BEGIN

void regAllStrategy() {
  StrategyAIP::regStrategy();
  StrategyBAH::regStrategy();
}

CR2END