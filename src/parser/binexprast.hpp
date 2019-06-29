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
    std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> mult_exprs;

  public:
    static ADD_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    ADD_EXPR_AST(std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> mult_exprs);

    std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> get_mult_exprs();
    void set_mult_exprs(std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> mult_exprs);

    ~ADD_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, ADD_EXPR_AST &expr);
};

class MULT_EXPR_AST : public AST
{
  private:
    std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> left_unary_exprs;

  public:
    static MULT_EXPR_AST*read(TOKEN**tok, LEXER*lexer);

    MULT_EXPR_AST(std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> left_unary_exprs);

    std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> get_left_unary_exprs();
    void set_left_unary_exprs(std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> left_unary_exprs);

    ~MULT_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, MULT_EXPR_AST &expr);
};

#endif  // BIN_EXPR_AST_HPP_INCLUDED
