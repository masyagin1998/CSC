#ifndef IDENT_TOKEN_HPP_INCLUDED
#define IDENT_TOKEN_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"

#include <string>
#include <ostream>

class IDENT_TOKEN : public TOKEN
{
private:
    std::string ident;

public:
    static IDENT_TOKEN*read(POSITION pos);
    
    IDENT_TOKEN(std::string ident, POSITION starting, POSITION following);

    std::string get_ident() const;

    friend std::ostream& operator<<(std::ostream &strm, const IDENT_TOKEN &tok);
};


#endif  // IDENT_TOKEN_HPP_INCLUDED
