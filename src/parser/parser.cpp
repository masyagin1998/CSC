#include "parser.hpp"

#include <iostream>

PARSER::PARSER(LEXER*lexer) {
    TOKEN*tok = lexer->next_token();
    unit = UNIT_AST::read(&tok, lexer);
}

UNIT_AST*PARSER::get_unit() { return unit; }

