#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/singleindicator.h>

CR2BEGIN

SingleIndicator::SingleIndicator(Wallet& wallet, Exchange& exchange,
                                 const char* nameAssets,
                                 const char* nameIndicator, int avgtimes)
    : Strategy(wallet, exchange), m_pIndicator(NULL), m_assetsName(nameAssets) {
  this->initIndicator(nameIndicator, avgtimes);
}

void SingleIndicator::initIndicator(const char* nameIndicator, int avgtimes) {
  auto mgr = IndicatorMgr::getSingleton();
  this->m_pIndicator = mgr->newIndicator(nameIndicator, avgtimes);
}

void SingleIndicator::onTimeStamp(TimeStamp ts, int index) {
  assert(this->m_pIndicator != NULL);
}

CR2END