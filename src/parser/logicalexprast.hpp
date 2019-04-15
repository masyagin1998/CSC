#ifndef LOGICAL_EXPR_AST_HPP_INCLUDED
#define LOGICAL_EXPR_AST_HPP_INCLUDED

#include "ast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class LOGICAL_AND_EXPR_AST;

class LOGICAL_OR_EXPR_AST : public AST
{
  private:
    LOGICAL_AND_EXPR_AST*first;
    LOGICAL_OR_EXPR_AST*second;

  public:
    static LOGICAL_OR_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    LOGICAL_OR_EXPR_AST(LOGICAL_AND_EXPR_AST*first, LOGICAL_OR_EXPR_AST*second);

    LOGICAL_AND_EXPR_AST*get_first();
    LOGICAL_OR_EXPR_AST*get_second();

    ~LOGICAL_OR_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, LOGICAL_OR_EXPR_AST &expr);
};

class EQ_EXPR_AST;

class LOGICAL_AND_EXPR_AST : public AST
{
  private:
    EQ_EXPR_AST*first;
    LOGICAL_AND_EXPR_AST*second;

  public:
    static LOGICAL_AND_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    LOGICAL_AND_EXPR_AST(EQ_EXPR_AST*first, LOGICAL_AND_EXPR_AST*second);

    EQ_EXPR_AST*get_first();
    LOGICAL_AND_EXPR_AST*get_second();

    ~LOGICAL_AND_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, LOGICAL_AND_EXPR_AST &expr);
};

#endif  // LOGICAL_EXPR_AST_HPP_INCLUDED
