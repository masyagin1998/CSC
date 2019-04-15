#ifndef IF_STMT_AST_HPP_INCLUDED
#define IF_STMT_AST_HPP_INCLUDED

#include "ast.hpp"
#include "logicalexprast.hpp"
#include "compoundstmtast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class ONLY_IF_STMT_AST;

class IF_STMT_AST : public AST
{
  private:
    ONLY_IF_STMT_AST*first;
    AST*second;

  public:
    static IF_STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    IF_STMT_AST(ONLY_IF_STMT_AST*first, AST*second);

    ONLY_IF_STMT_AST*get_first();
    AST*get_second();

    ~IF_STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm, IF_STMT_AST &expr);
};

class ONLY_IF_STMT_AST : public AST
{
  private:
    LOGICAL_OR_EXPR_AST*logical;
    COMPOUND_STMT_AST*comp_stmt;

  public:
    static ONLY_IF_STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    ONLY_IF_STMT_AST(LOGICAL_OR_EXPR_AST*logical, COMPOUND_STMT_AST*comp_stmt);

    LOGICAL_OR_EXPR_AST*get_logical();
    COMPOUND_STMT_AST*get_comp_stmt();

    ~ONLY_IF_STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm,ONLY_IF_STMT_AST &expr);
};

#endif  // IF_STMT_AST_HPP_INCLUDED
