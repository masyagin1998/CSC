#ifndef STMT_AST_HPP_INCLUDED
#define STMT_AST_HPP_INCLUDED

#include "ast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class STMT_AST : public AST
{
  private:
    AST*stmt;

  public:
    static STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    STMT_AST(AST*stmt);

    AST*get_stmt();

    ~STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm, STMT_AST &expr);
};

#endif  // STMT_AST_HPP_INCLUDED
