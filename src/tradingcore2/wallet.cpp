
#include <tradingcore2/exchange.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

Wallet::Wallet(Exchange& exchange) : m_exchange(exchange) {}

Assets Wallet::getAssets(const char* assetsName) const {
  auto it = this->m_map.find(assetsName);
  if (it != this->m_map.end()) {
    return it->second;
  }

  return Assets();
}

Assets* Wallet::_getAssets(const char* assetsName) {
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

void Wallet::deposit(Money money) {
  assert(money > ZEROMONEY);

  this->m_money += money;

  WalletHistoryNode n;
  n.setDeposit(money);

  this->_addHistory(n);
}

void Wallet::withdraw(Money money) {
  assert(money > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  this->m_money -= money;

  WalletHistoryNode n;
  n.setWithdraw(-money);

  this->_addHistory(n);
}

void Wallet::buyAssets(const char* assetsName, Money money, TimeStamp ts) {
  assert(assetsName != NULL);
  assert(money > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  Volume volume = ZEROVOLUME;
  Money price = ZEROMONEY;
  Money fee = ZEROMONEY;

  bool isok =
      m_exchange.calculateVolume(assetsName, ts, money, volume, price, fee);
  assert(isok);
  assert(price > ZEROMONEY);

  auto assets = this->_getAssets(assetsName);
  assert(assets != NULL);

  assets->inPrice = (assets->inPrice * assets->volume + volume * price) /
                    (assets->volume + volume);
  assets->volume += volume;
  assets->feesPaid += fee;

  this->m_money -= money;

  WalletHistoryNode n;
  n.setTrade(TT_BUY, assetsName, money, volume, ts, -money);

  this->_addHistory(n);
}

void Wallet::sellAssets(const char* assetsName, Volume volume, TimeStamp ts) {
  assert(assetsName != NULL);
  assert(volume > ZEROVOLUME);

  auto assets = this->_getAssets(assetsName);
  assert(assets != NULL);

  if (volume > assets->volume) {
    volume = assets->volume;
  }

  Money money = ZEROMONEY;
  Money price = ZEROMONEY;
  Money fee = ZEROMONEY;

  bool isok =
      m_exchange.calculatePrice(assetsName, ts, volume, money, price, fee);
  assert(isok);

  assets->volume -= volume;
  assets->feesPaid += fee;

  this->m_money += money;

  WalletHistoryNode n;
  n.setTrade(TT_SELL, assetsName, money, volume, ts, money);

  this->_addHistory(n);
}

CR2END