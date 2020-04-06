#include <tradingcore2/config.h>
#include <yaml-cpp/yaml.h>

CR2BEGIN

bool loadConfig(Config& cfg, const char* fn) {
  auto node = YAML::LoadFile(fn);
  if (node.IsNull()) {
    return false;
  }

  if (node["cnfundpath"].IsScalar()) {
    cfg.cnfundpath = node["cnfundpath"].as<std::string>();
  }

  if (node["exchanges"].IsSequence()) {
    for (auto it = node["exchanges"].begin(); it != node["exchanges"].end();
         ++it) {
      if (it->IsScalar()) {
        cfg.exchanges.push_back(it->as<std::string>());
      }
    }
  }

  return true;
}

CR2END