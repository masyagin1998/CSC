#ifndef COMPOUND_STMT_AST_HPP_INCLUDED
#define COMPOUND_STMT_AST_HPP_INCLUDED

#include "ast.hpp"
#include "declast.hpp"
#include "stmtast.hpp"

#include "lexer.hpp"

#include <vector>

class COMPOUND_STMT_AST : public AST
{
  private:
    std::vector<DECL_AST*> decls;
    std::vector<STMT_AST*> stmts;

  public:
    static COMPOUND_STMT_AST*read(TOKEN**tok, LEXER*lexer);
    
    COMPOUND_STMT_AST(std::vector<DECL_AST*> decls, std::vector<STMT_AST*> stmts);

    std::vector<DECL_AST*> get_decls();
    void set_decls(std::vector<DECL_AST*> decls);
    
    std::vector<STMT_AST*> get_stmts();
    void set_stmts(std::vector<STMT_AST*> stmts);

    ~COMPOUND_STMT_AST();

    friend std::ostream& operator<<(std::ostream &strm, COMPOUND_STMT_AST &stmt);
};

#endif  // COMPOUND_STMT_AST_HPP_INCLUDED
