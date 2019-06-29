#ifndef DECL_AST_HPP_INCLUDED
#define DECL_AST_HPP_INCLUDED

#include "ast.hpp"
#include "datatypeast.hpp"
#include "identast.hpp"
#include "logicalexprast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class DECL_AST : public AST
{
  private:
    DATATYPE_AST*datatype;
    IDENT_AST*ident;
    LOGICAL_OR_EXPR_AST*expr;

  public:
    static DECL_AST*read(TOKEN**tok, LEXER*lexer);
    
    DECL_AST(DATATYPE_AST*datatype, IDENT_AST*ident, LOGICAL_OR_EXPR_AST*expr);

    DATATYPE_AST*get_datatype();
    IDENT_AST*get_ident();
    LOGICAL_OR_EXPR_AST*get_expr();

    friend std::ostream& operator<<(std::ostream &strm, DECL_AST &decl);

    ~DECL_AST();
};

#endif  // DECL_AST_HPP_INCLUDED
