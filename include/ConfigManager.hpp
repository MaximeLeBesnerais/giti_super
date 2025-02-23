/*  ConfigManager.hppby: Maxime Le Besnerais
  mail: maxoulebesnerais@gmail.com
  created: 24/02/2025 00:15:38                   */

#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP
#include "ConfigReader.hpp"

class ConfigManager
{
private:
    ConfigParser &config;

public:
    explicit ConfigManager(ConfigParser &config);
    bool initialize();
    static std::string getConfigPath();
};

#endif // CONFIGMANAGER_HPP
