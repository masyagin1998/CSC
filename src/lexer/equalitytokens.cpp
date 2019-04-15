#include "equalitytokens.hpp"

const std::string EQUALITY_TOKEN::EQ = "==";
const std::string EQUALITY_TOKEN::NEQ = "!=";

DOMAIN_TAG EQUALITY_TOKEN::is(POSITION pos)
{
    if ((pos.get_code() == '=') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::EQ;
    } else if ((pos.get_code() == '!') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::NEQ;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

EQUALITY_TOKEN*EQUALITY_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string equality;

    switch (tag) {
    case DOMAIN_TAG::EQ:
        equality = EQ;
        break;
    case DOMAIN_TAG::NEQ:
        equality = NEQ;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < equality.size()) {
        i++;
        pos = pos.next();
    }

    return new EQUALITY_TOKEN(tag, starting, pos, equality);
}

EQUALITY_TOKEN::EQUALITY_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string equality) : TOKEN(tag, starting, following), equality(equality) {}

std::ostream& operator<<(std::ostream &strm, const EQUALITY_TOKEN &tok)
{
    return strm << "EQUALITY " << ((TOKEN) tok) << ": " << tok.equality;
}
