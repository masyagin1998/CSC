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
    ADD_EXPR_AST*first;
    DOMAIN_TAG*rel_op;
    REL_EXPR_AST*second;

  public:
    static REL_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    REL_EXPR_AST(ADD_EXPR_AST*first, DOMAIN_TAG*rel_op, REL_EXPR_AST*second);

    ADD_EXPR_AST*get_first();
    DOMAIN_TAG*get_rel_op();
    REL_EXPR_AST*get_second();

    ~REL_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, REL_EXPR_AST &expr);
};

#endif  // REL_EXPR_AST_HPP_INCLUDED
