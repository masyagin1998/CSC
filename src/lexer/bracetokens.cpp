#include "bracetokens.hpp"

const std::string BRACE_TOKEN::LBRACE = "{";
const std::string BRACE_TOKEN::RBRACE = "}";

DOMAIN_TAG BRACE_TOKEN::is(POSITION pos)
{
    if (pos.get_code() == '{') {
        return DOMAIN_TAG::LBRACE;
    } else if (pos.get_code() == '}') {
        return DOMAIN_TAG::RBRACE;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

BRACE_TOKEN*BRACE_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string brace;

    switch (tag) {
    case DOMAIN_TAG::LBRACE:
        brace = LBRACE;
        break;
    case DOMAIN_TAG::RBRACE:
        brace = RBRACE;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    pos = pos.next();

    return new BRACE_TOKEN(tag, starting, pos, brace);
}

BRACE_TOKEN::BRACE_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string brace) : TOKEN(tag, starting, following), brace(brace) {}

std::string BRACE_TOKEN::get_brace() const {
    return brace;
}

std::ostream& operator<<(std::ostream &strm, const BRACE_TOKEN &tok)
{
    return strm << "BRACE " << ((TOKEN) tok) << ": " << tok.brace;
}
