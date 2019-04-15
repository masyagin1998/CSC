#ifndef LOGICAL_TOKENS_HPP_INCLUDED
#define LOGICAL_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class LOGICAL_TOKEN : public TOKEN
{
  private:
    static const std::string LOGICAL_AND;
    static const std::string LOGICAL_OR;
    static const std::string LOGICAL_NOT;
    
    std::string logical;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static LOGICAL_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    LOGICAL_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string brace);

    std::string get_logical() const;

    friend std::ostream& operator<<(std::ostream &strm, const LOGICAL_TOKEN &tok);
};

#endif  // LOGICAL_TOKENS_HPP_INCLUDED
