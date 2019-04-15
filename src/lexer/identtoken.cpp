#include "identtoken.hpp"
#include "domaintag.hpp"

IDENT_TOKEN*IDENT_TOKEN::read(POSITION pos)
{
    POSITION starting = pos;
    std::string ident = "";
    if (pos.is_letter()) {
        ident += pos.get_code();
        pos = pos.next();
    } else {
        // TODO: throw exception.
    }

    while (pos.is_decimal_digit() || pos.is_letter()) {
        ident += pos.get_code();
        pos = pos.next();
    }

    return new IDENT_TOKEN(ident, starting, pos);
}

IDENT_TOKEN::IDENT_TOKEN(std::string ident, POSITION starting, POSITION following) : TOKEN(DOMAIN_TAG::IDENT, starting, following), ident(ident) {}

std::string IDENT_TOKEN::get_ident() const
{
    return ident;
}

std::ostream& operator<<(std::ostream &strm, const IDENT_TOKEN &tok)
{
    return strm << "IDENT " << ((TOKEN) tok) << ": " << tok.ident;
}
