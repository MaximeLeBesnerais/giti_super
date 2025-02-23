/*  ConfigReader.hpp by: Maxime Le Besnerais                              
  mail: maxoulebesnerais@gmail.com                              
  created: 23/02/2025 22:19:04                   */

#ifndef CONFIG_READER_HPP_
    #define CONFIG_READER_HPP_
    #define ISNUM(c) (c >= '0' && c <= '9')
    #define ISALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    #define ISALNUM(c) (ISNUM(c) || ISALPHA(c))
    #define STR_ISNUM(str) (str.find_first_not_of("0123456789") == std::string::npos)
    #define STR_ISALPHA(str) (str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos)
    #define STR_ISALNUM(str) (str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") == std::string::npos)

    #define CONF_LOC_ENV "GITI_CONFIG"
    #define CONF_LOC_ENV_DEFAULT "~/.giti/config.json"

    #include <vector>
    #include <string>
    #include <json.hpp>
    #include <cstdlib>

    static std::string getConfigPath() {
        const char* env_path = std::getenv(CONF_LOC_ENV);
        return env_path ? std::string(env_path) : std::string(CONF_LOC_ENV_DEFAULT);
    }

    typedef struct Date_s {
        std::string day;
        std::string month;
        std::string year;
    } Date;
    
    typedef struct Version_struct{
        std::string VersionName;
        std::string Changelog;
        std::string major;
        std::string minor;
        std::string patch;
    } Version;

    typedef struct file_setup_struct {
        std::vector<std::string> build_file_ext;
        std::vector<std::string> ignore_file_ext;
        std::vector<std::string> other_file_ext;
    } file_setup;

    typedef struct commit_settings_struct {
        bool require_scope;
        std::vector<std::string> allowed_types;
        bool allow_custom_types;
        bool require_issue_reference;
        std::string issue_prefix;
    } commit_settings;    

    typedef struct Config_struct{
        Version version;
        std::string author;
        Date LastUpdate;
        file_setup _file_setup;
        commit_settings _commit_settings;
    } Config;

    class ConfigParser {
    private:
        nlohmann::json config_json;
        Config config;

        void parseVersion(const nlohmann::json& j);
        void parseDate(const nlohmann::json& j);
        void parseFileSetup(const nlohmann::json& j);
        void parseCommitSettings(const nlohmann::json& j);

    public:
        ConfigParser() = default;
        ~ConfigParser() = default;

        bool loadConfig(const std::string& filepath);
        Config getConfig() const;
        static bool createDefaultConfig(const std::string& filepath);
        void printConfig() const;
    };

#endif // CONFIG_READER_HPP_
