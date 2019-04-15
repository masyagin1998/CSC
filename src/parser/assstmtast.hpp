#ifndef ASS_STMT_AST_HPP_INCLUDED
#define ASS_STMT_AST_HPP_INCLUDED

#include "ast.hpp"
#include "identast.hpp"
#include "logicalexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class ASS_STMT_AST : public AST
{
  private:
    IDENT_AST*ident;
    DOMAIN_TAG*ass_op;
    LOGICAL_OR_EXPR_AST*expr;

  public:
    static ASS_STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    ASS_STMT_AST(IDENT_AST*ident, DOMAIN_TAG*ass_op, LOGICAL_OR_EXPR_AST*expr);

    IDENT_AST*get_ident();
    DOMAIN_TAG*get_ass_op();
    LOGICAL_OR_EXPR_AST*get_expr();

    ~ASS_STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm, ASS_STMT_AST &ass_stmt);
};

#endif  // ASS_STMT_AST_HPP_INCLUDED
