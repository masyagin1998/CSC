#ifndef PRIMARY_EXPR_AST_HPP_INCLUDED
#define PRIMARY_EXPR_AST_HPP_INCLUDED

#include "ast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class PRIMARY_EXPR_AST : public AST
{
  private:
    AST*primary_expr;

  public:
    static PRIMARY_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    PRIMARY_EXPR_AST(AST*primary_expr);

    AST*get_primary_expr();

    ~PRIMARY_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, PRIMARY_EXPR_AST &expr);
};

#endif  // PRIMARY_EXPR_AST_HPP_INCLUDED
