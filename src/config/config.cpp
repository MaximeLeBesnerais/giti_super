// src/config/Config.cpp
/*
 * Author: Maxime Le Besnerais
 * Created: 2/24/2025
 * License: MIT
 * GitHub: https://github.com/MaximeLeBesnerais
 */

#include "giti/config/config.hpp"
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace giti
{
    namespace config
    {
        class Config::Impl
        {
        public:
            nlohmann::json config_json;
            Version version;
            std::string author;
            Date last_update;
            FileSetup file_setup;
            CommitSettings commit_settings;

            void parseVersion(const nlohmann::json &j)
            {
                version.name = j["version"]["VersionName"];
                version.changelog = j["version"]["Changelog"];
                version.major = j["version"]["major"];
                version.minor = j["version"]["minor"];
                version.patch = j["version"]["patch"];
            }

            void parseDate(const nlohmann::json &j)
            {
                last_update.day = j["LastUpdate"]["day"];
                last_update.month = j["LastUpdate"]["month"];
                last_update.year = j["LastUpdate"]["year"];
            }

            void parseFileSetup(const nlohmann::json &j)
            {
                file_setup.build_file_ext =
                    j["file_setup"]["build_file_ext"].get<std::vector<std::string>>();
                file_setup.ignore_file_ext =
                    j["file_setup"]["ignore_file_ext"].get<std::vector<std::string>>();
                file_setup.other_file_ext =
                    j["file_setup"]["other_file_ext"].get<std::vector<std::string>>();
            }

            void parseCommitSettings(const nlohmann::json &j)
            {
                commit_settings.require_scope =
                    j["commit_settings"]["require_scope"];
                commit_settings.allowed_types =
                    j["commit_settings"]["allowed_types"].get<std::vector<std::string>>();
                commit_settings.allow_custom_types =
                    j["commit_settings"]["allow_custom_types"];
                commit_settings.require_issue_reference =
                    j["commit_settings"]["require_issue_reference"];
                commit_settings.issue_prefix =
                    j["commit_settings"]["issue_prefix"];
            }
        };

        // Constructor & Destructor
        Config::Config() : pImpl(std::make_unique<Impl>()) {}
        Config::~Config() = default;

        // Move operations
        Config::Config(Config &&) noexcept = default;
        Config &Config::operator=(Config &&) noexcept = default;

        // Public methods
        bool Config::loadConfig(const std::string &filepath)
        {
            try
            {
                std::ifstream config_file(filepath);
                if (!config_file.is_open())
                {
                    std::cerr << "Error: Could not open config file: " << filepath << std::endl;
                    std::cerr << "Some functionalities might not work properly without a config file." << std::endl;
                    std::cerr << "If needed, you can create a default config file by following the documentation." << std::endl;
                    return false;
                }

                config_file >> pImpl->config_json;

                pImpl->parseVersion(pImpl->config_json);
                pImpl->author = pImpl->config_json["author"];
                pImpl->parseDate(pImpl->config_json);
                pImpl->parseFileSetup(pImpl->config_json);
                pImpl->parseCommitSettings(pImpl->config_json);

                return true;
            }
            catch (const nlohmann::json::exception &e)
            {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
                return false;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error parsing config: " << e.what() << std::endl;
                return false;
            }
        }

        bool Config::createDefaultConfig(const std::string &filepath)
        {
            nlohmann::json default_config = {
                {"version", {{"VersionName", "Giti CPP Overload"}, {"Changelog", "Entire rewrite of Giti in Cpp, addition of new features, and bug fixes."}, {"major", "3"}, {"minor", "0"}, {"patch", "0"}}},
                {"author", "Maxime Le Besnerais"},
                {"LastUpdate", {{"day", "21"}, {"month", "02"}, {"year", "2025"}}},
                {"file_setup", {{"build_file_ext", {"Makefile", "CMakeLists.txt", "*.mk"}}, {"ignore_file_ext", {".gitignore", ".dockerignore"}}, {"other_file_ext", {".h", ".hpp", ".c", ".cpp"}}}},
                {"commit_settings", {{"require_scope", false}, {"allowed_types", {"feat", "fix", "docs", "style", "refactor", "test", "chore"}}, {"allow_custom_types", true}, {"require_issue_reference", false}, {"issue_prefix", "#"}}}};

            try
            {
                std::filesystem::path config_path(filepath);
                std::filesystem::create_directories(config_path.parent_path());

                std::ofstream config_file(filepath);
                if (!config_file.is_open())
                {
                    std::cerr << "Error: Could not create config file: " << filepath << std::endl;
                    return false;
                }

                config_file << default_config.dump(2);
                return true;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error creating default config: " << e.what() << std::endl;
                return false;
            }
        }

        // Getters
        Version Config::getVersion() const { return pImpl->version; }
        std::string Config::getAuthor() const { return pImpl->author; }
        Date Config::getLastUpdate() const { return pImpl->last_update; }
        FileSetup Config::getFileSetup() const { return pImpl->file_setup; }
        CommitSettings Config::getCommitSettings() const { return pImpl->commit_settings; }

        void Config::printConfig() const
        {
            const auto &v = pImpl->version;
            std::cout << "Version: " << v.name << std::endl;
            std::cout << "Changelog: " << v.changelog << std::endl;
            std::cout << "Major: " << v.major << std::endl;
            std::cout << "Minor: " << v.minor << std::endl;
            std::cout << "Patch: " << v.patch << std::endl;
            std::cout << "Author: " << pImpl->author << std::endl;

            const auto &d = pImpl->last_update;
            std::cout << "Last update: " << d.day << "/" << d.month << "/" << d.year << std::endl;

            std::cout << "Build file extensions: " << std::endl;
            for (const auto &ext : pImpl->file_setup.build_file_ext)
                std::cout << "\t" << ext << std::endl;

            std::cout << "Ignore file extensions: " << std::endl;
            for (const auto &ext : pImpl->file_setup.ignore_file_ext)
                std::cout << "\t" << ext << std::endl;

            std::cout << "Other file extensions: " << std::endl;
            for (const auto &ext : pImpl->file_setup.other_file_ext)
                std::cout << "\t" << ext << std::endl;

            const auto &cs = pImpl->commit_settings;
            std::cout << "Require scope: " << cs.require_scope << std::endl;
            std::cout << "Allowed types: " << std::endl;
            for (const auto &type : cs.allowed_types)
                std::cout << "\t" << type << std::endl;
            std::cout << "Allow custom types: " << cs.allow_custom_types << std::endl;
            std::cout << "Require issue reference: " << cs.require_issue_reference << std::endl;
            std::cout << "Issue prefix: " << cs.issue_prefix << std::endl;
        }
    }
}