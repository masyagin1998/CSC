#ifndef EQUALITY_TOKENS_HPP_INCLUDED
#define EQUALITY_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class EQUALITY_TOKEN : public TOKEN
{
  private:
    static const std::string EQ;
    static const std::string NEQ;
    
    std::string equality;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static EQUALITY_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    EQUALITY_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string equality);

    std::string get_equality() const;

    friend std::ostream& operator<<(std::ostream &strm, const EQUALITY_TOKEN &tok);
};

#endif  // EQUALITY_TOKENS_HPP_INCLUDED
