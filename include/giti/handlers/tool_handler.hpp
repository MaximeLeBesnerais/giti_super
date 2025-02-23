// include/giti/handlers/tool_handler.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/


#ifndef GITI_HANDLERS_TOOL_HANDLER_HPP_
#define GITI_HANDLERS_TOOL_HANDLER_HPP_

#include "giti/cli/types.hpp"
#include "giti/config/config.hpp"
#include <memory>

namespace giti {
namespace handlers {

class ToolHandler {
public:
    // Constructor & Destructor
    ToolHandler(const config::Config& config);
    ~ToolHandler();
    
    // Delete copy operations
    ToolHandler(const ToolHandler&) = delete;
    ToolHandler& operator=(const ToolHandler&) = delete;
    
    // Move operations
    ToolHandler(ToolHandler&&) noexcept;
    ToolHandler& operator=(ToolHandler&&) noexcept;

    // Main handler method
    bool handle(const cli::ToolFlags& flags);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace handlers
} // namespace giti

#endif // GITI_HANDLERS_TOOL_HANDLER_HPP_
