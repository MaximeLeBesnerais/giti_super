// include/giti/config/config.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#ifndef GITI_CONFIG_HPP_
    #define GITI_CONFIG_HPP_

    #include <string>
    #include <vector>
    #include <memory>

    namespace giti {
    namespace config {

    // These structs can stay public as they're just data structures
    struct Version {
        std::string name;
        std::string changelog;
        std::string major;
        std::string minor;
        std::string patch;
    };

    struct Date {
        std::string day;
        std::string month;
        std::string year;
    };

    struct FileSetup {
        std::vector<std::string> build_file_ext;
        std::vector<std::string> ignore_file_ext;
        std::vector<std::string> other_file_ext;
    };

    struct CommitSettings {
        bool require_scope;
        std::vector<std::string> allowed_types;
        bool allow_custom_types;
        bool require_issue_reference;
        std::string issue_prefix;
    };

    class Config {
    public:
        // Constructor & Destructor
        Config();
        ~Config();
        
        // Delete copy operations
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
        
        // Allow move operations
        Config(Config&&) noexcept;
        Config& operator=(Config&&) noexcept;

        // Public interface
        bool loadConfig(const std::string& filepath);
        static bool createDefaultConfig(const std::string& filepath);
        void printConfig() const;

        // Getters
        Version getVersion() const;
        std::string getAuthor() const;
        Date getLastUpdate() const;
        FileSetup getFileSetup() const;
        CommitSettings getCommitSettings() const;

    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };

    } // namespace config
    } // namespace giti

#endif // GITI_CONFIG_HPP_