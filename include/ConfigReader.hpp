/*
Name: ConfigReader.hpp                     
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu            
description:                               
created: 21/02/2025 00:40:19              
*/

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
    #include "json.hpp"
    
    typedef struct Date_s {
        std::string day;
        std::string month;
        std::string year;
    } Date;
    
    /**
     * @brief Struct to store the version of the program
     */
    typedef struct Version_struct{
        std::string VersionName;
        std::string Changelog;
        std::string major;
        std::string minor;
        std::string patch;
    } Version;

    /**
     * @brief Control what files are selected by giti when using shortcuts
     * 
     * @default build_file_ext: ["Makefile", "CMakeLists.txt", "*.mk"]
     * @default ignore_file_ext: [".gitignore", ".dockerignore"]
     * @default other_file_ext: [".h", ".hpp", ".c", ".cpp"]
     */
    typedef struct file_setup_struct {
        std::vector<std::string> build_file_ext;
        std::vector<std::string> ignore_file_ext;
        std::vector<std::string> other_file_ext;
    } file_setup;

    /**
     * @brief Control what is required in a commit, how it should be formatted
     * 
     * @param require_scope: false - to be implemented
     * @param allowed_types: [] (empty vector) - enforce a list of types
     * @param allow_custom_types: true if allowed_types is empty, false otherwise
     * @param require_issue_reference: false - to be implemented
     * @param issue_prefix: "#", "JIRA-" - to be implemented
     */
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
        file_setup file_setup;
        commit_settings commit_settings;
    } Config;

#endif // CONFIG_READER_HPP_
