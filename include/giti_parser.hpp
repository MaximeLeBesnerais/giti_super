/*
Name: giti_parser.h                        
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu               
description:                               
created: 20/02/2025 03:18:42              
*/

#ifndef GITI_PARSER_H_
    #define GITI_PARSER_H_
    #include <cxxopts.hpp>
    cxxopts::Options createUtilityParser(void);
    cxxopts::Options createToolParser(void);
    cxxopts::Options createCommandLineParser(void);
#endif /* !GITI_PARSER_H_ */
