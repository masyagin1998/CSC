#ifndef UNARY_EXPR_AST_HPP_INCLUDED
#define UNARY_EXPR_AST_HPP_INCLUDED

#include "ast.hpp"
#include "primaryexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class RIGHT_UNARY_EXPR_AST;

class LEFT_UNARY_EXPR_AST : public AST
{
  private:
    DOMAIN_TAG*left_unary_op;
    PRIMARY_EXPR_AST*first;

  public:
    static LEFT_UNARY_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    LEFT_UNARY_EXPR_AST(DOMAIN_TAG*left_unary_op, PRIMARY_EXPR_AST*first);

    DOMAIN_TAG*get_left_unary_op();
    PRIMARY_EXPR_AST*get_first();

    ~LEFT_UNARY_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, LEFT_UNARY_EXPR_AST &expr);
};

// Not implemented yet.
class RIGHT_UNARY_EXPR_AST : public AST
{
  private:
    DOMAIN_TAG*right_unary_op;
    PRIMARY_EXPR_AST*first;

  public:
    static RIGHT_UNARY_EXPR_AST*read(TOKEN**tok, LEXER*lexer);

    RIGHT_UNARY_EXPR_AST(DOMAIN_TAG*right_unary_op, PRIMARY_EXPR_AST*first);

    DOMAIN_TAG*get_right_unary_op();
    PRIMARY_EXPR_AST*get_first();

    ~RIGHT_UNARY_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, RIGHT_UNARY_EXPR_AST &expr);
};

#endif  // UNARY_EXPR_AST_HPP_INCLUDED
