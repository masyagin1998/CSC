#ifndef ARG_AST_HPP_INCLUDED
#define ARG_AST_HPP_INCLUDED

#include "ast.hpp"
#include "datatypeast.hpp"
#include "identast.hpp"

#include <vector>

class ARG_AST : public AST
{
  private:
    DATATYPE_AST*datatype;
    IDENT_AST*ident;

  public:
    static ARG_AST*read(TOKEN**tok, LEXER*lexer);
    
    ARG_AST(DATATYPE_AST*datatype, IDENT_AST*ident);

    DATATYPE_AST*get_datatype();
    IDENT_AST*get_ident();

    ~ARG_AST();

    friend std::ostream& operator<<(std::ostream &strm, ARG_AST &arg);
};

#endif  // ARG_AST_HPP_INCLUDED
