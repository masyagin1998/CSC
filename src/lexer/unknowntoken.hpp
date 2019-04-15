#ifndef UNKNOWN_TOKEN_HPP_INCLUDED
#define UNKNOWN_TOKEN_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class UNKNOWN_TOKEN : public TOKEN
{
private:
    std::string unknown;
    
public:
    static UNKNOWN_TOKEN*read(POSITION pos);

    UNKNOWN_TOKEN(DOMAIN_TAG tag, std::string unknown, POSITION starting, POSITION following);

    std::string get_unknown() const;

    friend std::ostream& operator<<(std::ostream &strm, const UNKNOWN_TOKEN &tok);
};

#endif  // UNKNOWN_TOKEN_HPP_INCLUDED
