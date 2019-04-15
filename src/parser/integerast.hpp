#ifndef INTEGER_AST_HPP_INCLUDED
#define INTEGER_AST_HPP_INCLUDED

#include "ast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class INTEGER_AST : public AST
{
  private:
    int integer;

  public:
    static INTEGER_AST*read(TOKEN*tok, LEXER*lexer);  
    
    INTEGER_AST(int integer);

    int get_integer() const;
    void set_integer(int integer);

    friend std::ostream& operator<<(std::ostream &strm, INTEGER_AST &integer);
};

#endif  // INTEGER_AST_HPP_INCLUDED
