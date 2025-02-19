/*
Name: giti_parser_result.hpp               
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu       
description:                               
created: 20/02/2025 04:01:34               
*/

#ifndef GITI_PARSER_RESULT_HPP_
    #define GITI_PARSER_RESULT_HPP_
    #include <string>
    #include <vector>
    typedef struct command_line_s {
        std::string title;
        std::string message;
        std::string type;
        std::vector<std::string> files;
    } cmd;
#endif /* !GITI_PARSER_RESULT_HPP_ */