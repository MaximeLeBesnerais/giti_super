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
#include <filesystem>
#include <fstream>

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
        std::cout << "🔄 Updating Giti..." << std::endl;
        
        try {
            // Create temporary directory for update
            std::filesystem::path tempDir = std::filesystem::temp_directory_path() / "giti_update";
            std::filesystem::create_directories(tempDir);
    
            // Create a temporary script that will:
            // 1. Move the new binary to a temporary location
            // 2. Kill the current giti process
            // 3. Move the new binary to the correct location
            std::string updateScript = 
                "#!/bin/bash\n"
                "cd " + tempDir.string() + " && \n"
                "if [ -d \"giti\" ]; then\n"
                "    cd giti && git pull\n"
                "else\n"
                "    git clone https://github.com/MaximeLeBesnerais/giti_super.git giti\n"
                "    cd giti\n"
                "fi && \n"
                "# Replace the old config.json with the new one\n"
                "sudo cp config.json $HOME/.giti/config.json && \n"
                "make && \n"
                "# Create a temporary copy of the new binary\n"
                "sudo cp build/giti /usr/bin/giti.new && \n"
                "# Get the PID of the current giti process (our process)\n"
                "GITI_PID=$$ && \n"
                "# Create a background process that will:\n"
                "# 1. Wait for our process to exit\n"
                "# 2. Move the new binary into place\n"
                "# 3. Clean up\n"
                "(while kill -0 $GITI_PID 2>/dev/null; do sleep 0.1; done; \n"
                " sudo mv /usr/bin/giti.new /usr/bin/giti; \n"
                " rm -rf " + tempDir.string() + ") & \n"
                "exit 0\n";
    
            // Write update script to temporary file
            std::filesystem::path scriptPath = tempDir / "update.sh";
            std::ofstream scriptFile(scriptPath);
            scriptFile << updateScript;
            scriptFile.close();
    
            // Make script executable
            std::filesystem::permissions(scriptPath, 
                std::filesystem::perms::owner_exec | 
                std::filesystem::perms::owner_read | 
                std::filesystem::perms::owner_write,
                std::filesystem::perm_options::add);
    
            // Execute update script
            std::string cmd = "bash " + scriptPath.string();
            int result = std::system(cmd.c_str());
    
            if (result != 0) {
                std::cerr << "❌ Update failed. Please check your permissions and try again." << std::endl;
                return false;
            }
    
            std::cout << "✅ Giti has been successfully updated! The changes will take effect next time you run giti." << std::endl;
            return true;
    
        } catch (const std::exception& e) {
            std::cerr << "❌ Update failed: " << e.what() << std::endl;
            return false;
        }
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