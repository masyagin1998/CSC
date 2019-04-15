#ifndef LOOP_STMT_AST_HPP_INCLUDED
#define LOOP_STMT_AST_HPP_INCLUDED

#include "ast.hpp"
#include "logicalexprast.hpp"
#include "compoundstmtast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class WHILE_STMT_AST : public AST
{
  private:
    LOGICAL_OR_EXPR_AST*logical;
    COMPOUND_STMT_AST*comp_stmt;

  public:
    static WHILE_STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    WHILE_STMT_AST(LOGICAL_OR_EXPR_AST*logical, COMPOUND_STMT_AST*comp_stmt);

    LOGICAL_OR_EXPR_AST*get_logical();
    COMPOUND_STMT_AST*get_comp_stmt();

    ~WHILE_STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm, WHILE_STMT_AST &expr);
};

#endif  // LOOP_STMT_AST_HPP_INCLUDED
