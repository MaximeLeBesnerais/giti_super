/*
Name: giti_parsers.cpp                     
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu            
description:                               
created: 20/02/2025 03:19:03              
*/

#include <giti_parser.hpp>
/**
 * @brief Create an utility parser for file selection
 * @return The utility parser
 */
cxxopts::Options createUtilityParser(void) {
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

/**
 * @brief Create a Tool parser for special commands
 * @return The Tool parser
 */
cxxopts::Options createToolParser(void) {
    cxxopts::Options options("Giti tools", "Tools for special commands");
    options.add_options()
        ("v,version", "Print current version of giti")
        ("u,update", "Update giti to the latest version")
        ("f,force", "Reinstall giti to the latest version")
        ("r,remove", "Get rid of giti and all its files");
    options.allow_unrecognised_options();
    return options;
}

/**
 * @brief Create a command line parser for the program
 * @return The command line parser
 */
cxxopts::Options createCommandLineParser(void) {
    cxxopts::Options options("Giti", "A git utility for better commits");
    options.add_options()
        ("t,title", "Commit title", cxxopts::value<std::string>())
        ("m,message", "Commit message", cxxopts::value<std::string>());

    options.add_options()
        ("type", "Type of the commit", cxxopts::value<std::string>())
        ("files", "Files to commit", cxxopts::value<std::vector<std::string>>());

    options.parse_positional({"type", "files"});
    options.positional_help("<type> [files...]");

    return options;
}

_utilityParser utilityManager(cxxopts::ParseResult result) {
    _utilityParser utility;
    utility._all = result.count("all") > 0;
    utility._build = result.count("build") > 0;
    utility._ignore = result.count("ignore") > 0;
    utility._del = result.count("del") > 0;
    utility._other = result.count("other") > 0;
    return utility;
}

_toolParser toolManager(cxxopts::ParseResult result) {
    _toolParser tool;
    tool._version = result.count("version") > 0;
    tool._update = result.count("update") > 0;
    tool._force = result.count("force") > 0;
    tool._remove = result.count("remove") > 0;
    return tool;
}

bool parserAnyTrue(cxxopts::ParseResult result, _toolParser tool) {
    tool._version = result.count("version") > 0;
    tool._update = result.count("update") > 0;
    tool._force = result.count("force") > 0;
    tool._remove = result.count("remove") > 0;
    return tool._version || tool._update || tool._force || tool._remove;
}

bool parserAnyTrue(cxxopts::ParseResult result, _utilityParser utility) {
    utility._all = result.count("all") > 0;
    utility._build = result.count("build") > 0;
    utility._ignore = result.count("ignore") > 0;
    utility._del = result.count("del") > 0;
    utility._other = result.count("other") > 0;
    return utility._all || utility._build || utility._ignore || utility._del || utility._other;
}

void printFound(_toolParser tool) {
    if (tool._version)
        std::cout << "Version" << std::endl;
    if (tool._update)
        std::cout << "Update" << std::endl;
    if (tool._force)
        std::cout << "Force" << std::endl;
    if (tool._remove)
        std::cout << "Remove" << std::endl;
}

void printFound(_utilityParser utility) {
    if (utility._all)
        std::cout << "All" << std::endl;
    if (utility._build)
        std::cout << "Build" << std::endl;
    if (utility._ignore)
        std::cout << "Ignore" << std::endl;
    if (utility._del)
        std::cout << "Del" << std::endl;
    if (utility._other)
        std::cout << "Other" << std::endl;
}
