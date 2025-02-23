// src/handlers/tool_handler.cpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#include "giti/handlers/tool_handler.hpp"
#include <iostream>
#include <unistd.h>

namespace giti {
namespace handlers {

class ToolHandler::Impl {
public:
    Impl(const config::Config& cfg) : config(cfg) {}

    bool handleVersion() const {
        auto version = config.getVersion();
        std::cout << "Giti Version:\n" << version.name << " ("
                  << version.major << "."
                  << version.minor << "."
                  << version.patch << ")" << std::endl;
        std::cout << "Changelog: " << version.changelog << std::endl;
        return true;
    }

    bool handleUpdate() const {
        std::cout << "Update functionality not yet implemented." << std::endl;
        std::cout << "To update, please check the repository for new versions." << std::endl;
        return true;
    }

    bool handleForce() const {
        std::cout << "Force reinstall functionality not yet implemented." << std::endl;
        std::cout << "To reinstall, please follow the installation instructions in the documentation." << std::endl;
        return true;
    }

    bool handleRemove() const {
        if (geteuid() != 0) {
            std::cerr << "You must be root to remove Giti." << std::endl;
            return false;
        }

        std::cout << "Removing Giti..." << std::endl;
        if (std::remove("/usr/bin/giti") != 0) {
            std::cerr << "❌ Failed to remove Giti." << std::endl;
            return false;
        }
        std::cout << "Giti executable removed." << std::endl;

        std::string home = std::getenv("HOME");
        std::string gitiDir = home + "/.giti";
        if (std::remove(gitiDir.c_str()) != 0) {
            std::cerr << "❌ Failed to remove Giti directory." << std::endl;
            return false;
        }
        std::cout << "Giti directory removed." << std::endl;
        std::cout << "✅ Giti has been successfully removed." << std::endl;
        return true;
    }

    const config::Config& config;
};

// Constructor & Destructor
ToolHandler::ToolHandler(const config::Config& config) 
    : pImpl(std::make_unique<Impl>(config)) {}
ToolHandler::~ToolHandler() = default;

// Move operations
ToolHandler::ToolHandler(ToolHandler&&) noexcept = default;
ToolHandler& ToolHandler::operator=(ToolHandler&&) noexcept = default;

bool ToolHandler::handle(const cli::ToolFlags& flags) {
    if (flags.version) {
        return pImpl->handleVersion();
    }
    if (flags.update) {
        return pImpl->handleUpdate();
    }
    if (flags.force) {
        return pImpl->handleForce();
    }
    if (flags.remove) {
        return pImpl->handleRemove();
    }
    return false;
}

} // namespace handlers
} // namespace giti