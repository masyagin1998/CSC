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
    REL_EXPR_AST*first;
    DOMAIN_TAG*eq_op;
    EQ_EXPR_AST*second;

  public:
    static EQ_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    EQ_EXPR_AST(REL_EXPR_AST*first, DOMAIN_TAG*eq_op, EQ_EXPR_AST*second);

    REL_EXPR_AST*get_first();
    DOMAIN_TAG*get_eq_op();
    EQ_EXPR_AST*get_second();

    ~EQ_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, EQ_EXPR_AST &expr);
};

#endif  // EQ_EXPR_AST_HPP_INCLUDED
