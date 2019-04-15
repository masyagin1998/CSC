#ifndef UNIT_AST_HPP_INCLUDED
#define UNIT_AST_HPP_INCLUDED

#include "ast.hpp"
#include "funcast.hpp"

#include "lexer.hpp"

#include <vector>

class UNIT_AST : public AST
{
  private:
    std::vector<FUNC_AST*> funcs;

  public:
    static UNIT_AST*read(TOKEN**tok, LEXER*lexer);
    
    UNIT_AST(std::vector<FUNC_AST*> funcs);

    std::vector<FUNC_AST*> get_funcs();
    void set_funcs(std::vector<FUNC_AST*> funcs);

    ~UNIT_AST();

    friend std::ostream& operator<<(std::ostream &strm, UNIT_AST &unit);
};

#endif  // UNIT_AST_HPP_INCLUDED
