#ifndef REL_EXPR_AST_HPP_INCLUDED
#define REL_EXPR_AST_HPP_INCLUDED

#include "ast.hpp"
#include "binexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class PRIMARY_EXPR_AST;

class REL_EXPR_AST;

class REL_EXPR_AST : public AST
{
  private:
    std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> add_exprs;

  public:
    static REL_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    REL_EXPR_AST(std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> rel_exprs);

    std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> get_add_exprs();
    void set_add_exprs(std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> add_exprs);

    ~REL_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, REL_EXPR_AST &expr);
};

#endif  // REL_EXPR_AST_HPP_INCLUDED
