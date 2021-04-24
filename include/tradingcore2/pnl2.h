#ifndef __TRADINGCORE2_PNL2_H__
#define __TRADINGCORE2_PNL2_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>

#include <map>
#include <set>
#include <string>
#include <vector>

CR2BEGIN

class PNL2 {
 public:
  PNL2() {}
  ~PNL2() { this->release(); }

 public:
  void release();

 public:
  void buyAsset(const char* market0, const char* symbol0, time_t ts,
                Money money, Money volume, Money fee, int moneyParts);

  void sellAsset(const char* market0, const char* symbol0, time_t ts,
                 Money money, Money volume, Money fee, int moneyParts);

  void want2buyAsset(const char* market0, const char* symbol0, time_t ts,
                     Money money, Money limitPrice);

  void want2sellAsset(const char* market0, const char* symbol0, time_t ts,
                      Money volume, Money limitPrice);

  void withdraw(Money money, time_t ts);

  void deposit(Money money, time_t ts);

 public:
  void initTimestamp(const Exchange& exchange);

  void onInitTimeStamp(const Exchange& exchange, TimeStamp ts, int index);

 public:
  void onBuildEnd(const Exchange& exchange);

 protected:
  void calcMaxDrawdown();

  void calcMaxDrawdown2();

  void calcMaxDrawup();

  void calcMaxDrawup2();

  void calcTotalReturns();

  void calcSharpe(const Exchange& exchange);

  void calcAnnualizedReturns(const Exchange& exchange);

  void calcAnnualizedVolatility(const Exchange& exchange);

  void calcAnnualizedReturns2(const Exchange& exchange);

  void calcAnnualizedVolatility2(const Exchange& exchange);

  void calcVariance();

  void calcControls();

  // 找到 starti 前面的最高点
  int findPreMax(int starti);
  // 找到 starti 后面的最低点
  int findNextMin(int starti);
  // 找到starti前面第一个阶段性低点
  // 假设starti是一个高点，该函数返回这个高点前一个下跌的终点
  int findPreUpMin(int starti);

  // 找到 starti 前面的最低点
  int findPreMin(int starti);
  // 找到 starti 后面的最高点
  int findNextMax(int starti);
  // 找到starti前面第一个阶段性高点
  // 假设starti是一个低点，该函数返回这个低点前一个上涨的终点
  int findPreDownMax(int starti);

 public:
  // 构建ctrl统计数据
  void onBuildCtrl(const Exchange& exchange);
  // 清理构建PNL时需要的统计数据
  void clearCtrlTmpData();

  void addAsset(const char* asset);

  void getAssetInfo(const Exchange& exchange, const char* asset, TimeStamp ts,
                    Money& cost, Volume& volume);

  void getAssetInfo2(const Exchange& exchange, const char* asset, TimeStamp ts,
                     Money& cost, Volume& volume);

  int getAssetInfo3(const Exchange& exchange, const char* asset, TimeStamp ts,
                    Money& cost, Volume& volume, int ctrlIndex);

  void getHandMoneyEx(const Exchange& exchange, TimeStamp ts, Money& total,
                      Money& last);

  void getHandMoneyEx2(const Exchange& exchange, TimeStamp ts, Money& total,
                       Money& last);

  int getHandMoneyEx3(const Exchange& exchange, TimeStamp ts, Money& total,
                      Money& last, int ctrlIndex);

  int setTotalPNLAssetData(const Exchange* pExchange,
                           ::tradingpb::PNLDataValue* pVal, int ctrlIndex);

  void procTotalPNLAssetData(const Exchange& exchange);
  // 处理ctrlnode里的统计数据
  void procCtrlNodeData(const Exchange& exchange);

 public:
  tradingpb::PNLData m_data;
  std::set<std::string> m_mapAssets;
};

CR2END

#endif  // __TRADINGCORE2_PNL2_H__