// include/giti/cli/types.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#ifndef GITI_CLI_TYPES_HPP_
#define GITI_CLI_TYPES_HPP_

#include <string>
#include <vector>

namespace giti {
namespace cli {

struct CommitCommand {
    std::string title;
    std::string message;
    std::string type;
    std::vector<std::string> files;
};

struct UtilityFlags {
    bool all = false;
    bool build = false;
    bool ignore = false;
    bool deleted = false;
    bool other = false;
};

struct ToolFlags {
    bool version = false;
    bool update = false;
    bool force = false;
    bool remove = false;
};

} // namespace cli
} // namespace giti

#endif // GITI_CLI_TYPES_HPP_
