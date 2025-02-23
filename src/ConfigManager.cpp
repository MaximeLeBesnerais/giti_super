/*  ConfigManager.cppby: Maxime Le Besnerais                              
  mail: maxoulebesnerais@gmail.com                              
  created: 24/02/2025 00:16:37                   */

#include "ConfigManager.hpp"

ConfigManager::ConfigManager(ConfigParser& config) : config(config) {}

bool ConfigManager::initialize() {
    return config.loadConfig(getConfigPath());
}