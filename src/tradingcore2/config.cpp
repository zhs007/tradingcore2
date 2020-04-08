#include <tradingcore2/config.h>
#include <yaml-cpp/yaml.h>

CR2BEGIN

bool loadConfig(Config& cfg, const char* fn) {
  auto node = YAML::LoadFile(fn);
  if (node.IsNull()) {
    return false;
  }

  if (node["cnfundpath"].IsScalar()) {
    cfg.cnfundPath = node["cnfundpath"].as<std::string>();
  }

  if (node["exchanges"].IsSequence()) {
    for (auto it = node["exchanges"].begin(); it != node["exchanges"].end();
         ++it) {
      if (it->IsScalar()) {
        cfg.exchanges.push_back(it->as<std::string>());
      }
    }
  }

  if (node["bindaddr"].IsScalar()) {
    cfg.bindaddr = node["bindaddr"].as<std::string>();
  }

  if (node["tasknums"].IsScalar()) {
    cfg.taskNums = node["tasknums"].as<int>();
  } else {
    cfg.taskNums = 0;
  }

  if (node["tokens"].IsSequence()) {
    for (auto it = node["tokens"].begin(); it != node["tokens"].end(); ++it) {
      if (it->IsScalar()) {
        cfg.tokens.push_back(it->as<std::string>());
      }
    }
  }

  if (node["servs"].IsSequence()) {
    for (auto it = node["servs"].begin(); it != node["servs"].end(); ++it) {
      if (it->IsScalar()) {
        cfg.servs.push_back(it->as<std::string>());
      }
    }
  }

  if (node["token"].IsScalar()) {
    cfg.token = node["token"].as<std::string>();
  }

  return true;
}

CR2END