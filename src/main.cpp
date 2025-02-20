/*
Name: main.cpp                             
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu               
description:                               
created: 20/02/2025 03:19:18              
*/

#include <iostream>
#include <giti_parser.hpp>
#include <giti_parser_result.hpp>

bool utilityManager(cxxopts::ParseResult result) {
    if (result.count("all")) {
        std::cout << "ALL" << std::endl;
        return true;
    }
    if (result.count("build")) {
        std::cout << "BUILD" << std::endl;
        return true;
    }
    if (result.count("ignore")) {
        std::cout << "IGNORE" << std::endl;
        return true;
    }
    if (result.count("del")) {
        std::cout << "DEL" << std::endl;
        return true;
    }
    return false;
}

bool toolManager(cxxopts::ParseResult result) {
    if (result.count("version")) {
        std::cout << "VERSION" << std::endl;
        return true;
    }
    if (result.count("update")) {
        std::cout << "UPDATE" << std::endl;
        return true;
    }
    if (result.count("force")) {
        std::cout << "FORCE" << std::endl;
        return true;
    }
    if (result.count("remove")) {
        std::cout << "REMOVE" << std::endl;
        return true;
    }
    if (result.count("help")) {
        std::cout << "HELP" << std::endl;
        return true;
    }
    return false;
}

bool commandLineManager(cxxopts::ParseResult result) {
    cmd command;
    command.title = result.count("title") > 0 
    ? result["title"].as<std::string>() : "No title";
    command.message = result.count("message") > 0
    ? result["message"].as<std::string>() : "No message";
    if (result.count("type"))
        command.type = result["type"].as<std::string>();
    if (result.count("files"))
        command.files = result["files"].as<std::vector<std::string>>();
    if (result.count("type") == 0 || result.count("files") == 0) {
        std::cerr << "Error: Missing data" << std::endl;
        if (result.count("type") == 0)
            std::cerr << "Type is missing" << std::endl;
        if (result.count("files") == 0)
            std::cerr << "Files are missing" << std::endl;
        return false;
    }
    std::cout << "Title: " <<  command.title << std::endl;
    std::cout << "Message: " << command.message << std::endl;
    std::cout << "Type: " << command.type << std::endl;
    std::cout << "Files: " << std::endl;
    for (auto &file : command.files)
        std::cout << "\t" << file << std::endl;
    return true;
}

int main(int argc, char *argv[]) {
    cxxopts::Options parser_used = createUtilityParser();
    cxxopts::ParseResult result = parser_used.parse(argc, argv);
    
    if (utilityManager(result))
        return 0;

    parser_used = createToolParser();
    result = parser_used.parse(argc, argv);
    
    if (toolManager(result))
        return 0;

    parser_used = createCommandLineParser();
    try {
        result = parser_used.parse(argc, argv);
        if (commandLineManager(result))
            return 0;
    } catch (const cxxopts::exceptions::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << "No command found" << std::endl;
    return 0;
}