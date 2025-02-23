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
    typedef struct {
        bool _all;
        bool _build;
        bool _ignore;
        bool _del;
        bool _other;
    } _utilityParser;
    
    typedef struct {
        bool _version;
        bool _update;
        bool _force;
        bool _remove;
    } _toolParser;
#endif /* !GITI_PARSER_H_ */
