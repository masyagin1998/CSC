#ifndef FUNC_AST_HPP_INCLUDED
#define FUNC_AST_HPP_INCLUDED

#include "ast.hpp"
#include "datatypeast.hpp"
#include "identast.hpp"
#include "argslistast.hpp"
#include "compoundstmtast.hpp"

#include <vector>
#include <utility>

class FUNC_AST : public AST
{
  private:
    DATATYPE_AST*datatype;
    IDENT_AST*ident;
    ARGS_LIST_AST*args_list; 
    COMPOUND_STMT_AST*body;

  public:
    static FUNC_AST*read(TOKEN**tok, LEXER*lexer);
    
    FUNC_AST(DATATYPE_AST*datatype,
             IDENT_AST*ident,
             ARGS_LIST_AST*args_list,
             COMPOUND_STMT_AST*body);

    DATATYPE_AST*get_datatype();
    IDENT_AST*get_ident();
    ARGS_LIST_AST*get_args_list(); 
    COMPOUND_STMT_AST*get_body();

    ~FUNC_AST();

    friend std::ostream& operator<<(std::ostream &strm, FUNC_AST &func);
};

#endif  // FUNC_AST_HPP_INCLUDED
