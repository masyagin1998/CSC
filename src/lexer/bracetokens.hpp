#ifndef BRACE_TOKENS_HPP_INCLUDED
#define BRACE_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class BRACE_TOKEN : public TOKEN
{
  private:
    static const std::string LBRACE;
    static const std::string RBRACE;
    
    std::string brace;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static BRACE_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    BRACE_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string brace);

    std::string get_brace() const;

    friend std::ostream& operator<<(std::ostream &strm, const BRACE_TOKEN &tok);
};

#endif  // BRACE_TOKENS_HPP_INCLUDED
