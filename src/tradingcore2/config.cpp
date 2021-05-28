#include <tradingcore2/config.h>
#include <yaml-cpp/yaml.h>

#include "yamlutils.h"

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
    cfg.taskNums = yamlGetScalar<int>(node["tasknums"], 0);
  } else {
    cfg.taskNums = 0;
  }

  if (node["islimittasks"].IsScalar()) {
    cfg.isLimitTasks = yamlGetScalar<bool>(node["islimittasks"], false);
  } else {
    cfg.isLimitTasks = false;
  }

  if (node["tasktimeoff"].IsScalar()) {
    cfg.taskTimeOff = yamlGetScalar<int>(node["tasktimeoff"], 0);
    if (cfg.taskTimeOff < 0) {
      cfg.taskTimeOff = 0;
    }
  } else {
    cfg.taskTimeOff = 0;
  }

  if (node["maxtasksnums"].IsScalar()) {
    cfg.maxTasksNums = yamlGetScalar<int>(node["maxtasksnums"], 0);
    if (cfg.maxTasksNums < 0) {
      cfg.maxTasksNums = 0;
    }
  } else {
    cfg.maxTasksNums = 0;
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
      if (it->IsMap()) {
        Config::ServerInfo si;

        if ((*it)["host"].IsScalar()) {
          si.host = (*it)["host"].as<std::string>();
        }

        if ((*it)["token"].IsScalar()) {
          si.token = (*it)["token"].as<std::string>();
        }

        cfg.servs.push_back(si);
      }
    }
  }

  if (node["trdb2serv"].IsScalar()) {
    cfg.trdb2Serv = node["trdb2serv"].as<std::string>();
  }

  if (node["trdb2token"].IsScalar()) {
    cfg.trdb2Token = node["trdb2token"].as<std::string>();
  }

  return true;
}

CR2END