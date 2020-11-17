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
                Money money, Money volume, Money fee);

  void sellAsset(const char* market0, const char* symbol0, time_t ts,
                 Money money, Money volume, Money fee);

  void withdraw(Money money, time_t ts);

  void deposit(Money money, time_t ts);

 public:
  void initTimestamp(const Exchange& exchange);

  void onInitTimeStamp(const Exchange& exchange, TimeStamp ts, int index);

 public:
  void procTimestamp(const Exchange& exchange, TimeStamp ts);

 public:
  void addAsset(const char* asset);

  Volume getAssetVolume(const Exchange& exchange, const char* asset,
                        TimeStamp ts);

 public:
  tradingpb::PNLData m_data;
  std::set<std::string> m_mapAssets;
};

CR2END

#endif  // __TRADINGCORE2_PNL2_H__