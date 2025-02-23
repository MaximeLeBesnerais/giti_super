// src/handlers/tool_handler.cpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#include "giti/handlers/tool_handler.hpp"
#include <iostream>
#include <filesystem>

namespace giti {
namespace handlers {

class ToolHandler::Impl {
public:
    Impl(const config::Config& cfg) : config(cfg) {}

    bool handleVersion() const {
        auto version = config.getVersion();
        std::cout << "Giti Version " << version.name << " ("
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
        std::cout << "Remove functionality not yet implemented." << std::endl;
        std::cout << "To remove manually:" << std::endl;
        std::cout << "1. Remove the executable" << std::endl;
        std::cout << "2. Remove configuration files from ~/.giti/" << std::endl;
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