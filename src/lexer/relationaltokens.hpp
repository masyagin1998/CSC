#ifndef RELATIONAL_TOKENS_HPP_INCLUDED
#define RELATIONAL_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class RELATIONAL_TOKEN : public TOKEN
{
  private:
    static const std::string MORE;
    static const std::string LESS;
    static const std::string MORE_OR_EQ;
    static const std::string LESS_OR_EQ;
    
    std::string relational;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static RELATIONAL_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    RELATIONAL_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string relational);

    std::string get_relational() const;

    friend std::ostream& operator<<(std::ostream &strm, const RELATIONAL_TOKEN &tok);
};

#endif  // RELATIONAL_TOKENS_HPP_INCLUDED
