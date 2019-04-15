#ifndef DATATYPE_AST_HPP_INCLUDED
#define DATATYPE_AST_HPP_INCLUDED

#include "ast.hpp"
#include "identast.hpp"

#include "lexer.hpp"

class DATATYPE_AST : public AST
{
  private:
    DOMAIN_TAG basic_datatype;
    IDENT_AST  *ident;

  public:
    static DATATYPE_AST*read(TOKEN**tok, LEXER*lexer);
    
    DATATYPE_AST(DOMAIN_TAG basic_datatype, IDENT_AST*ident);

    DOMAIN_TAG get_basic_datatype() const;
    void set_basic_datatype(DOMAIN_TAG basic_datatype);
    
    IDENT_AST*get_ident();

    ~DATATYPE_AST();

    friend std::ostream& operator<<(std::ostream &strm, DATATYPE_AST &datatype);
};

#endif  // DATATYPE_AST_HPP_INCLUDED
