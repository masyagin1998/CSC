#ifndef RET_STMT_AST_HPP_INCLUDED
#define RET_STMT_AST_HPP_INCLUDED

#include "ast.hpp"
#include "datatypeast.hpp"
#include "identast.hpp"
#include "logicalexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class RET_STMT_AST : public AST
{
  private:
    LOGICAL_OR_EXPR_AST*expr;

  public:
    static RET_STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    RET_STMT_AST(LOGICAL_OR_EXPR_AST*expr);
    LOGICAL_OR_EXPR_AST*get_expr();

    ~RET_STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm, RET_STMT_AST &ret_stmt);
};

#endif  // RET_STMT_AST_HPP_INCLUDED
