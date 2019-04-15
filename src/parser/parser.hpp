#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include "lexer.hpp"

#include "unitast.hpp"

class PARSER
{
  private:
    UNIT_AST*unit;
    
  public:
    PARSER(LEXER*lexer);

    UNIT_AST*get_unit();
};

#endif  // PARSER_HPP_INCLUDED
