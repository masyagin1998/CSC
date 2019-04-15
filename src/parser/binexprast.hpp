#ifndef BIN_EXPR_AST_HPP_INCLUDED
#define BIN_EXPR_AST_HPP_INCLUDED

#include "ast.hpp"
#include "unaryexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class MULT_EXPR_AST;

class ADD_EXPR_AST : public AST
{
  private:
    MULT_EXPR_AST*first;
    DOMAIN_TAG*add_op;
    ADD_EXPR_AST*second;

  public:
    static ADD_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    ADD_EXPR_AST(MULT_EXPR_AST*first, DOMAIN_TAG*add_op, ADD_EXPR_AST*second);

    MULT_EXPR_AST*get_first();
    DOMAIN_TAG*get_add_op();
    ADD_EXPR_AST*get_second();

    ~ADD_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, ADD_EXPR_AST &expr);
};

class MULT_EXPR_AST : public AST
{
  private:
    LEFT_UNARY_EXPR_AST*first;
    DOMAIN_TAG*mult_op;
    MULT_EXPR_AST*second;

  public:
    static MULT_EXPR_AST*read(TOKEN**tok, LEXER*lexer);

    MULT_EXPR_AST(LEFT_UNARY_EXPR_AST*first, DOMAIN_TAG*mult_op, MULT_EXPR_AST*second);

    LEFT_UNARY_EXPR_AST*get_first();
    DOMAIN_TAG*get_mult_op();
    MULT_EXPR_AST*get_second();

    ~MULT_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, MULT_EXPR_AST &expr);
};

#endif  // BIN_EXPR_AST_HPP_INCLUDED
