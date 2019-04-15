#ifndef IDENT_AST_HPP_INCLUDED
#define IDENT_AST_HPP_INCLUDED

#include "ast.hpp"

#include "lexer.hpp"

#include <vector>
#include <utility>

class IDENT_AST : public AST
{
  private:
    std::string ident;

  public:
    static IDENT_AST*read(TOKEN**tok, LEXER*lexer);
    
    IDENT_AST(std::string ident);

    std::string get_ident() const;
    void set_ident(std::string ident);

    friend std::ostream& operator<<(std::ostream &strm, IDENT_AST &ident);
};

#endif  // IDENT_AST_HPP_INCLUDED
