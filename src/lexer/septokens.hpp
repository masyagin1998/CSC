#ifndef SEP_TOKENS_HPP_INCLUDED
#define SEP_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class SEP_TOKEN : public TOKEN
{
  private:
    static const std::string SEMICOLON;
    static const std::string COMMA;
    
    std::string sep;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static SEP_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    SEP_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string sep);

    std::string get_sep() const;

    friend std::ostream& operator<<(std::ostream &strm, const SEP_TOKEN &tok);
};

#endif  // SEP_TOKENS_HPP_INCLUDED
