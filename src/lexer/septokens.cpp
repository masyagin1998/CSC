#include "septokens.hpp"

const std::string SEP_TOKEN::SEMICOLON = ";";
const std::string SEP_TOKEN::COMMA = ",";

DOMAIN_TAG SEP_TOKEN::is(POSITION pos)
{
    if (pos.get_code() == ';') {
        return DOMAIN_TAG::SEMICOLON;
    } else if (pos.get_code() == ',') {
        return DOMAIN_TAG::COMMA;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

SEP_TOKEN*SEP_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string sep;

    switch (tag) {
    case DOMAIN_TAG::SEMICOLON:
        sep = SEMICOLON;
        break;
    case DOMAIN_TAG::COMMA:
        sep = COMMA;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    pos = pos.next();

    return new SEP_TOKEN(tag, starting, pos, sep);
}

SEP_TOKEN::SEP_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string sep) : TOKEN(tag, starting, following), sep(sep) {}

std::string SEP_TOKEN::get_sep() const {
    return sep;
}

std::ostream& operator<<(std::ostream &strm, const SEP_TOKEN &tok)
{
    return strm << "SEP " << ((TOKEN) tok) << ": " << tok.sep;
}
