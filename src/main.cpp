/*
Name: main.cpp                             
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu               
description:                               
created: 20/02/2025 03:19:18              
*/

#include <iostream>
#include <giti_parser.hpp>

int main(int argc, char *argv[]) {
    cxxopts::Options parser_used = createUtilityParser();
    cxxopts::ParseResult result = parser_used.parse(argc, argv);
    // if any of the options are present, print "FOUND"
    if (result.count("all") || result.count("build") || result.count("ignore") || result.count("del"))
        std::cout << "FOUND UTILITY" << std::endl;

    parser_used = createToolParser();
    result = parser_used.parse(argc, argv);
    // if any of the options are present, print "FOUND"
    if (result.count("version") || result.count("update") || result.count("force") || result.count("remove") || result.count("help"))
        std::cout << "FOUND TOOL" << std::endl;

    parser_used = createCommandLineParser();
    try {
        result = parser_used.parse(argc, argv);
        // if any of the options are present, print "FOUND"
        if (result.count("title") || result.count("message") || result.count("type") || result.count("files"))
            std::cout << "FOUND COMMAND LINE" << std::endl;
    } catch (const cxxopts::exceptions::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}