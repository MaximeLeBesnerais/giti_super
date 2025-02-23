/*  CommandManager.hpp  by: Maxime Le Besnerais
  mail: maxoulebesnerais@gmail.com
  created: 24/02/2025 00:10:46                   */

#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP
#include <cxxopts.hpp>
#include "GitRepoClass.hpp"
#include "giti_parser_result.hpp"
#include "ConfigReader.hpp"

class CommandManager
{
private:
    GitRepository &repo;
    ConfigParser &config;
    cxxopts::Options utility_parser;
    cxxopts::Options tool_parser;
    cxxopts::Options command_parser;

    // Private methods for command handling
    bool handleUtilityCommands(const cxxopts::ParseResult &result);
    bool handleToolCommands(const cxxopts::ParseResult &result);
    bool handleGitCommands(const cxxopts::ParseResult &result);

    // Command validation
    bool validateCommand(const cmd &command) const;
    void printCommandDetails(const cmd &command) const;

public:
    CommandManager(GitRepository &repo, ConfigParser &config);
    bool processCommands(int argc, char *argv[]);
};

#endif // COMMANDMANAGER_HPP
