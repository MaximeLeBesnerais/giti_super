// include/giti/handlers/utility_handler.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#ifndef GITI_HANDLERS_UTILITY_HANDLER_HPP_
#define GITI_HANDLERS_UTILITY_HANDLER_HPP_

#include "giti/cli/types.hpp"
#include "giti/git/repository.hpp"
#include "giti/config/config.hpp"
#include <memory>

namespace giti {
namespace handlers {

class UtilityHandler {
public:
    UtilityHandler(git::Repository& repo, const config::Config& config);
    ~UtilityHandler();
    
    // Delete copy operations
    UtilityHandler(const UtilityHandler&) = delete;
    UtilityHandler& operator=(const UtilityHandler&) = delete;
    
    // Move operations
    UtilityHandler(UtilityHandler&&) noexcept;
    UtilityHandler& operator=(UtilityHandler&&) noexcept;

    bool handle(const cli::UtilityFlags& flags);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace handlers
} // namespace giti

#endif // GITI_HANDLERS_UTILITY_HANDLER_HPP_
