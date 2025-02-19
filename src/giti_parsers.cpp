
#include <cxxopts.hpp>

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
        ("d,del", "Add deleted files to the selection");
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
        ("r, remove", "Get rid of giti and all its files");
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
        ("m,message", "Commit message", cxxopts::value<std::string>())
        ("help", "Print help");

    options.add_options()
        ("type", "Type of the commit", cxxopts::value<std::string>())
        ("files", "Files to commit", cxxopts::value<std::vector<std::string>>());

    options.parse_positional({"type", "files"});
    options.positional_help("<type> [files...]");

    return options;
}