// src/cli/parser.cpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#include "giti/cli/parser.hpp"
#include <cxxopts.hpp>
#include <iostream>

namespace giti {
namespace cli {

class Parser::Impl {
public:
    cxxopts::Options createCommitParser() {
        cxxopts::Options options("Giti", "A git utility for better commits");
        options.add_options()
            ("t,title", "Commit title", cxxopts::value<std::string>())
            ("m,message", "Commit message", cxxopts::value<std::string>())
            ("type", "Type of the commit", cxxopts::value<std::string>())
            ("files", "Files to commit", cxxopts::value<std::vector<std::string>>());

        options.parse_positional({"type", "files"});
        options.positional_help("<type> [files...]");

        return options;
    }

    cxxopts::Options createUtilityParser() {
        cxxopts::Options options("Giti utility", "Utility for file selection");
        options.add_options()
            ("a,all", "Select all files")
            ("b,build", "Makefiles, CMakeLists.txt or other build files")
            ("i,ignore", ".gitignore, .dockerignore or other ignore files")
            ("d,del", "Add deleted files to the selection")
            ("o,other", "Add other files to the selection (to configure)");
        options.allow_unrecognised_options();
        return options;
    }

    cxxopts::Options createToolParser() {
        cxxopts::Options options("Giti tools", "Tools for special commands");
        options.add_options()
            ("v,version", "Print current version of giti")
            ("u,update", "Update giti to the latest version")
            ("f,force", "Reinstall giti to the latest version")
            ("r,remove", "Get rid of giti and all its files");
        options.allow_unrecognised_options();
        return options;
    }
};

// Constructor & Destructor
Parser::Parser() : pImpl(std::make_unique<Impl>()) {}
Parser::~Parser() = default;

// Move operations
Parser::Parser(Parser&&) noexcept = default;
Parser& Parser::operator=(Parser&&) noexcept = default;

std::optional<CommitCommand> Parser::parseCommit(int argc, char* argv[]) {
    try {
        auto options = pImpl->createCommitParser();
        auto result = options.parse(argc, argv);

        CommitCommand cmd;
        cmd.title = result.count("title") > 0 ? 
            result["title"].as<std::string>() : "No title";
        cmd.message = result.count("message") > 0 ? 
            result["message"].as<std::string>() : "No message";

        if (result.count("type") == 0 || result.count("files") == 0)
            return std::nullopt;

        cmd.type = result["type"].as<std::string>();
        cmd.files = result["files"].as<std::vector<std::string>>();

        return cmd;
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing commit command: " << e.what() << std::endl;
        return std::nullopt;
    }
}

UtilityFlags Parser::parseUtility(int argc, char* argv[]) {
    auto options = pImpl->createUtilityParser();
    auto result = options.parse(argc, argv);

    UtilityFlags flags;
    flags.all = result.count("all") > 0;
    flags.build = result.count("build") > 0;
    flags.ignore = result.count("ignore") > 0;
    flags.deleted = result.count("del") > 0;
    flags.other = result.count("other") > 0;

    return flags;
}

ToolFlags Parser::parseTool(int argc, char* argv[]) {
    auto options = pImpl->createToolParser();
    auto result = options.parse(argc, argv);

    ToolFlags flags;
    flags.version = result.count("version") > 0;
    flags.update = result.count("update") > 0;
    flags.force = result.count("force") > 0;
    flags.remove = result.count("remove") > 0;

    return flags;
}

void Parser::printCommitHelp() const {
    auto options = pImpl->createCommitParser();
    std::cout << options.help() << std::endl;
}

void Parser::printUtilityHelp() const {
    auto options = pImpl->createUtilityParser();
    std::cout << options.help() << std::endl;
}

void Parser::printToolHelp() const {
    auto options = pImpl->createToolParser();
    std::cout << options.help() << std::endl;
}

} // namespace cli
} // namespace giti
