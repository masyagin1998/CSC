#ifndef INTEGER_TOKEN_HPP_INCLUDED
#define INTEGER_TOKEN_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"

#include <string>

class INTEGER_TOKEN : public TOKEN
{
private:
    std::string integer_str;
    int         integer_val;

public:
    static INTEGER_TOKEN*read(POSITION pos);
    
    INTEGER_TOKEN(std::string integer_str, POSITION starting, POSITION following);

    int get_integer() const;

    friend std::ostream& operator<<(std::ostream &strm, const INTEGER_TOKEN &tok);
};


#endif  // INTEGER_TOKEN_HPP_INCLUDED
