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
  std::string cnfundPath;

  // for server
  std::string bindaddr;
  int taskNums;
  std::vector<std::string> tokens;

  bool hasToken(const char* strToken) const {
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
      if (*it == strToken) {
        return true;
      }
    }

    return false;
  }

  // for client
  std::vector<std::string> servs;
  std::string token;
};

bool loadConfig(Config& cfg, const char* fn);

CR2END

#endif  // __TRADINGCORE2_CONFIG_H__