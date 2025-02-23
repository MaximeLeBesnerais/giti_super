// include/giti/cli/parsers.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#ifndef GITI_CLI_PARSER_HPP_
#define GITI_CLI_PARSER_HPP_

#include "giti/cli/types.hpp"
#include <memory>
#include <optional>

namespace giti {
namespace cli {

class Parser {
public:
    // Constructor & Destructor
    Parser();
    ~Parser();
    
    // Delete copy operations
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    
    // Move operations
    Parser(Parser&&) noexcept;
    Parser& operator=(Parser&&) noexcept;

    // Parse methods
    std::optional<CommitCommand> parseCommit(int argc, char* argv[]);
    UtilityFlags parseUtility(int argc, char* argv[]);
    ToolFlags parseTool(int argc, char* argv[]);

    // Help methods
    void printCommitHelp() const;
    void printUtilityHelp() const;
    void printToolHelp() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace cli
} // namespace giti

#endif // GITI_CLI_PARSER_HPP_
