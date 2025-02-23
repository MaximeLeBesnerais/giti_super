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
#include "giti/handlers/tool_handler.hpp"
#include "giti/handlers/utility_handler.hpp"
#include "giti/handlers/commit_handler.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Initialize configuration
        giti::config::Config config;
        config.loadConfig(getConfigPath());

        // Parse commands
        giti::cli::Parser parser;
        
        // Handle tool flags first (these don't need repo access)
        auto toolFlags = parser.parseTool(argc, argv);
        if (toolFlags.version || toolFlags.update || toolFlags.force || toolFlags.remove) {
            giti::handlers::ToolHandler handler(config);
            return handler.handle(toolFlags) ? 0 : 1;
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
            giti::handlers::UtilityHandler handler(repo, config);
            return handler.handle(utilityFlags) ? 0 : 1;
        }

        // Handle commit command
        if (auto commitCmd = parser.parseCommit(argc, argv)) {
            giti::handlers::CommitHandler handler(repo, config);
            return handler.handle(*commitCmd) ? 0 : 1;
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
