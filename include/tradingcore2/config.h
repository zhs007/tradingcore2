#ifndef __TRADINGCORE2_CONFIG_H__
#define __TRADINGCORE2_CONFIG_H__

#include <tradingcore2/basedef.h>

#include <string>
#include <vector>

CR2BEGIN

struct Config {
  std::vector<std::string> exchanges;

  std::string cnfundpath;

  std::string bindaddr;

  std::vector<std::string> servs;
};

bool loadConfig(Config& cfg, const char* fn);

CR2END

#endif  // __TRADINGCORE2_CONFIG_H__