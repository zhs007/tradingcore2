#ifndef __TRADINGCORE2_SRC_YAMLUTILS_H__
#define __TRADINGCORE2_SRC_YAMLUTILS_H__

#include <tradingcore2/basedef.h>
#include <yaml-cpp/yaml.h>

CR2BEGIN

template <typename ValType>
ValType yamlGetScalar(const YAML::Node& node, ValType defaultVal) {
  assert(node.IsScalar());

  try {
    return node.as<ValType>();
  } catch (...) {
    return defaultVal;
  }
};

CR2END

#endif  // __TRADINGCORE2_SRC_YAMLUTILS_H__