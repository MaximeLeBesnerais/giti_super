// ConfigReader.cpp

#include "ConfigReader.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

void ConfigParser::parseVersion(const nlohmann::json& j) {
    config.version.VersionName = j["version"]["VersionName"];
    config.version.Changelog = j["version"]["Changelog"];
    config.version.major = j["version"]["major"];
    config.version.minor = j["version"]["minor"];
    config.version.patch = j["version"]["patch"];
}

void ConfigParser::parseDate(const nlohmann::json& j) {
    config.LastUpdate.day = j["LastUpdate"]["day"];
    config.LastUpdate.month = j["LastUpdate"]["month"];
    config.LastUpdate.year = j["LastUpdate"]["year"];
}

void ConfigParser::parseFileSetup(const nlohmann::json& j) {
    config._file_setup.build_file_ext = 
        j["file_setup"]["build_file_ext"].get<std::vector<std::string>>();
    config._file_setup.ignore_file_ext = 
        j["file_setup"]["ignore_file_ext"].get<std::vector<std::string>>();
    config._file_setup.other_file_ext = 
        j["file_setup"]["other_file_ext"].get<std::vector<std::string>>();
}

void ConfigParser::parseCommitSettings(const nlohmann::json& j) {
    config._commit_settings.require_scope = 
        j["commit_settings"]["require_scope"];
    config._commit_settings.allowed_types = 
        j["commit_settings"]["allowed_types"].get<std::vector<std::string>>();
    config._commit_settings.allow_custom_types = 
        j["commit_settings"]["allow_custom_types"];
    config._commit_settings.require_issue_reference = 
        j["commit_settings"]["require_issue_reference"];
    config._commit_settings.issue_prefix = 
        j["commit_settings"]["issue_prefix"];
}

bool ConfigParser::loadConfig(const std::string& filepath) {
    try {
        std::ifstream config_file(filepath);
        if (!config_file.is_open()) {
            std::cerr << "Error: Could not open config file: " << filepath << std::endl;
            return false;
        }

        config_file >> config_json;

        parseVersion(config_json);
        config.author = config_json["author"];
        parseDate(config_json);
        parseFileSetup(config_json);
        parseCommitSettings(config_json);

        return true;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing config: " << e.what() << std::endl;
        return false;
    }
}

Config ConfigParser::getConfig() const {
    return config;
}

bool ConfigParser::createDefaultConfig(const std::string& filepath) {
    nlohmann::json default_config = {
        {"version", {
            {"VersionName", "Giti CPP Overload"},
            {"Changelog", "Entire rewrite of Giti in Cpp, addition of new features, and bug fixes."},
            {"major", "3"},
            {"minor", "0"},
            {"patch", "0"}
        }},
        {"author", "Maxime Le Besnerais"},
        {"LastUpdate", {
            {"day", "21"},
            {"month", "02"},
            {"year", "2025"}
        }},
        {"file_setup", {
            {"build_file_ext", {"Makefile", "CMakeLists.txt", "*.mk"}},
            {"ignore_file_ext", {".gitignore", ".dockerignore"}},
            {"other_file_ext", {".h", ".hpp", ".c", ".cpp"}}
        }},
        {"commit_settings", {
            {"require_scope", false},
            {"allowed_types", {"feat", "fix", "docs", "style", "refactor", "test", "chore"}},
            {"allow_custom_types", true},
            {"require_issue_reference", false},
            {"issue_prefix", "#"}
        }}
    };

    try {
        std::filesystem::path config_path(filepath);
        std::filesystem::create_directories(config_path.parent_path());

        std::ofstream config_file(filepath);
        if (!config_file.is_open()) {
            std::cerr << "Error: Could not create config file: " << filepath << std::endl;
            return false;
        }

        config_file << default_config.dump(2);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating default config: " << e.what() << std::endl;
        return false;
    }
}
