#ifndef __TRADINGCORE2_CONFIG_H__
#define __TRADINGCORE2_CONFIG_H__

#include <tradingcore2/basedef.h>

#include <string>
#include <vector>

CR2BEGIN

struct Config {
  // for exchangemgr
  std::vector<std::string> exchanges;

  // for cnfund
  std::string cnfundpath;

  // for server
  std::string bindaddr;

  // for client
  std::vector<std::string> servs;
};

bool loadConfig(Config& cfg, const char* fn);

CR2END

#endif  // __TRADINGCORE2_CONFIG_H__