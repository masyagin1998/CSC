#include "unknowntoken.hpp"

UNKNOWN_TOKEN*UNKNOWN_TOKEN::read(POSITION pos)
{
    POSITION starting = pos;
    std::string unknown = "";
    while (pos.is_unknown() && !pos.is_EOF()) {
        unknown += pos.get_code();
        pos = pos.next();
    }
    return new UNKNOWN_TOKEN(DOMAIN_TAG::UNKNOWN, unknown, starting, pos);
}

UNKNOWN_TOKEN::UNKNOWN_TOKEN(DOMAIN_TAG tag, std::string unknown, POSITION starting, POSITION following) : TOKEN(tag, starting, following), unknown(unknown) {}

std::string UNKNOWN_TOKEN::get_unknown() const
{
    return unknown;
}

std::ostream& operator<<(std::ostream &strm, const UNKNOWN_TOKEN &tok)
{
    return strm << "UNKNOWN " << ((TOKEN) tok) << ": " << tok.unknown;
}
