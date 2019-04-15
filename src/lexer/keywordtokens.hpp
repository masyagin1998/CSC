#ifndef KEYWORD_TOKENS_HPP_INCLUDED
#define KEYWORD_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"
#include "position.hpp"

#include <string>
#include <ostream>

class KEYWORD_TOKEN : public TOKEN
{
  private:
    static const std::string RETURN;
    static const std::string FOR;
    static const std::string WHILE;
    static const std::string DO;
    static const std::string IF;
    static const std::string ELSE;
    
    std::string keyword;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static KEYWORD_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    KEYWORD_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string keyword);

    std::string get_keyword() const;

    friend std::ostream& operator<<(std::ostream &strm, const KEYWORD_TOKEN &tok);
};

#endif  // KEYWORD_TOKENS_HPP_INCLUDED
