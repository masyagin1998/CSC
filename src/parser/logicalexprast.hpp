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
    std::vector<LOGICAL_AND_EXPR_AST*> and_exprs;

  public:
    static LOGICAL_OR_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    LOGICAL_OR_EXPR_AST(std::vector<LOGICAL_AND_EXPR_AST*> and_exprs);

    std::vector<LOGICAL_AND_EXPR_AST*> get_and_exprs();
    void set_and_exprs(std::vector<LOGICAL_AND_EXPR_AST*> and_exprs);

    ~LOGICAL_OR_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, LOGICAL_OR_EXPR_AST &expr);
};

class EQ_EXPR_AST;

class LOGICAL_AND_EXPR_AST : public AST
{
  private:
    std::vector<EQ_EXPR_AST*> eq_exprs;

  public:
    static LOGICAL_AND_EXPR_AST*read(TOKEN**tok, LEXER*lexer);
    
    LOGICAL_AND_EXPR_AST(std::vector<EQ_EXPR_AST*> eq_exprs);

    std::vector<EQ_EXPR_AST*> get_eq_exprs();
    void set_eq_exprs(std::vector<EQ_EXPR_AST*> eq_exprs);

    ~LOGICAL_AND_EXPR_AST();

    friend std::ostream& operator<<(std::ostream &strm, LOGICAL_AND_EXPR_AST &expr);
};

#endif  // LOGICAL_EXPR_AST_HPP_INCLUDED
