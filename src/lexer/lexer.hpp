#ifndef LEXER_HPP_INCLUDED
#define LEXER_HPP_INCLUDED

#include "token.hpp"
#include "keywordtokens.hpp"
#include "datatypetokens.hpp"
#include "bracetokens.hpp"
#include "parentokens.hpp"
#include "septokens.hpp"
#include "identtoken.hpp"
#include "integertoken.hpp"
#include "equalitytokens.hpp"
#include "logicaltokens.hpp"
#include "relationaltokens.hpp"
#include "assigntokens.hpp"
#include "binoptokens.hpp"
#include "commenttoken.hpp"
#include "unknowntoken.hpp"

#include <string>

class LEXER
{
  private:
    std::string*program;

    bool skip_comments; // If true, lexer's next_token() won't return COMMENT_TOKEN.

    POSITION curr;

  public:
    LEXER(std::string fname);

    void enable_skip_comments();
    void disable_skip_comments();

    TOKEN*next_token();

    ~LEXER();
};

#endif  // LEXER_HPP_INCLUDED

