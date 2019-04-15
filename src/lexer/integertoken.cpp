#include "integertoken.hpp"

#include <string>
#include <ostream>
#include <cstring>

INTEGER_TOKEN*INTEGER_TOKEN::read(POSITION pos)
{
    POSITION starting = pos;
    std::string integer_str = "";
    while (pos.is_decimal_digit()) {
        integer_str += pos.get_code();
        pos = pos.next();
    }

    if (pos.get_code() != 'x') {
        return new INTEGER_TOKEN(integer_str, starting, pos);
    }

    integer_str += pos.get_code();
    pos = pos.next();

    while (pos.is_decimal_digit() || pos.is_letter()) {
        integer_str += pos.get_code();
        pos = pos.next();
    }

    return new INTEGER_TOKEN(integer_str, starting, pos);
}

INTEGER_TOKEN::INTEGER_TOKEN(std::string integer_str, POSITION starting, POSITION following) : TOKEN(DOMAIN_TAG::INTEGER, starting, following), integer_str(integer_str)
{
    if (integer_str.find("x") == std::string::npos) {
        integer_val = std::stoi(integer_str);
        return;
    }

    std::size_t n = 0;
    int base = std::stoi(integer_str, &n);
    integer_val = std::stoi(integer_str.substr(n+1, integer_str.size() - 1), NULL, base);
}

int INTEGER_TOKEN::get_integer() const
{
    return integer_val;
}

std::ostream& operator<<(std::ostream &strm, const INTEGER_TOKEN &tok)
{
    return strm << "INTEGER " << ((TOKEN) tok) << ": " << tok.integer_str;
}

