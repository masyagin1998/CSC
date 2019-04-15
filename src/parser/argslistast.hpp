#ifndef ARGS_LIST_AST_HPP_INCLUDED
#define ARGS_LIST_AST_HPP_INCLUDED

#include "ast.hpp"
#include "argast.hpp"

#include "lexer.hpp"

#include <vector>

class ARGS_LIST_AST : public AST
{
  private:
    std::vector<ARG_AST*> args_list;

  public:
    static ARGS_LIST_AST*read(TOKEN**tok, LEXER*lexer);
    
    ARGS_LIST_AST(std::vector<ARG_AST*> args_list);

    std::vector<ARG_AST*> get_args_list();
    void set_args_list(std::vector<ARG_AST*> args_list);

    ~ARGS_LIST_AST();

    friend std::ostream& operator<<(std::ostream &strm, ARGS_LIST_AST &args_list);
};

#endif  // ARGS_LIST_AST_HPP_INCLUDED
