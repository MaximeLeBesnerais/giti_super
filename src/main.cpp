// src/main.cpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#include "giti/cli/parser.hpp"
#include "giti/git/repository.hpp"
#include "giti/config/config.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Initialize configuration
        giti::config::Config config;
        config.loadConfig(getConfigPath());

        // Parse commands
        giti::cli::Parser parser;

        // Handle tool flags
        auto toolFlags = parser.parseTool(argc, argv);
        if (toolFlags.version || toolFlags.update || toolFlags.force || toolFlags.remove) {
            // Handle tool commands...
            return 0;
        }

        // Initialize git repository
        giti::git::Repository repo;
        if (!repo.open()) {
            std::cerr << "Error: Could not open repository" << std::endl;
            return 1;
        }

        // Handle utility flags
        auto utilityFlags = parser.parseUtility(argc, argv);
        if (utilityFlags.all || utilityFlags.build || utilityFlags.ignore || 
            utilityFlags.deleted || utilityFlags.other) {
            // Handle utility commands...
            return 0;
        }

        // Handle commit command
        if (auto commitCmd = parser.parseCommit(argc, argv)) {
            giti::git::Repository::CommitOptions options{
                .title = commitCmd->title,
                .message = commitCmd->message,
                .files = commitCmd->files
            };
            if (repo.commit(options)) {
                std::cout << "Successfully committed changes" << std::endl;
                return 0;
            }
            return 1;
        }

        // If no valid command was found
        std::cout << "No valid command found. Available commands:" << std::endl;
        parser.printCommitHelp();
        parser.printUtilityHelp();
        parser.printToolHelp();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}