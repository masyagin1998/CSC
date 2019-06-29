#ifndef EQ_EXPR_AST_HPP_INCLUDED
#define EQ_EXPR_AST_HPP_INCLUDED

#include "ast.hpp"
#include "relexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>
 
class EQ_EXPR_AST : public AST
{
  private:
    std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> rel_exprs;

  public:
    static EQ_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    EQ_EXPR_AST(std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> rel_exprs);

    std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> get_rel_exprs();
    void set_rel_exprs(std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> rel_exprs);

    ~EQ_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, EQ_EXPR_AST &expr);
};

#endif  // EQ_EXPR_AST_HPP_INCLUDED
