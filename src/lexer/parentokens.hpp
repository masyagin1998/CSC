#ifndef PAREN_TOKENS_HPP_INCLUDED
#define PAREN_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class PAREN_TOKEN : public TOKEN
{
  private:
    static const std::string LPAREN;
    static const std::string RPAREN;
    
    std::string paren;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static PAREN_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    PAREN_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string paren);

    std::string get_paren() const;

    friend std::ostream& operator<<(std::ostream &strm, const PAREN_TOKEN &tok);
};

#endif  // PAREN_TOKENS_HPP_INCLUDED
