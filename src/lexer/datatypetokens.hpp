#ifndef DATATYPE_TOKENS_HPP_INCLUDED
#define DATATYPE_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"
#include "position.hpp"

#include <string>
#include <ostream>

class DATATYPE_TOKEN : public TOKEN
{
  private:
    static const std::string INT;
    
    std::string datatype;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static DATATYPE_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    DATATYPE_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string datatype);

    std::string get_datatype() const;

    friend std::ostream& operator<<(std::ostream &strm, const DATATYPE_TOKEN &tok);
};

#endif  // DATATYPE_TOKENS_HPP_INCLUDED
