#ifndef ASSIGN_TOKENS_HPP_INCLUDED
#define ASSIGN_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class ASSIGN_TOKEN : public TOKEN
{
  private:
    static const std::string ASSIGN;
    static const std::string ASSIGN_PLUS;
    static const std::string ASSIGN_MINUS;
    static const std::string ASSIGN_MUL;
    static const std::string ASSIGN_DIV;
    static const std::string ASSIGN_MOD;
    
    std::string assign;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static ASSIGN_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    ASSIGN_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string brace);

    std::string get_assign() const;

    friend std::ostream& operator<<(std::ostream &strm, const ASSIGN_TOKEN &tok);
};

#endif  // ASSIGN_TOKENS_HPP_INCLUDED
