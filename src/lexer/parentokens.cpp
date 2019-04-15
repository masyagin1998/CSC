#include "parentokens.hpp"

const std::string PAREN_TOKEN::LPAREN = "(";
const std::string PAREN_TOKEN::RPAREN = ")";

DOMAIN_TAG PAREN_TOKEN::is(POSITION pos)
{
    if (pos.get_code() == '(') {
        return DOMAIN_TAG::LPAREN;
    } else if (pos.get_code() == ')') {
        return DOMAIN_TAG::RPAREN;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

PAREN_TOKEN*PAREN_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string paren;

    switch (tag) {
    case DOMAIN_TAG::LPAREN:
        paren = LPAREN;
        break;
    case DOMAIN_TAG::RPAREN:
        paren = RPAREN;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    pos = pos.next();

    return new PAREN_TOKEN(tag, starting, pos, paren);
}

PAREN_TOKEN::PAREN_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string paren) : TOKEN(tag, starting, following), paren(paren) {}

std::string PAREN_TOKEN::get_paren() const {
    return paren;
}

std::ostream& operator<<(std::ostream &strm, const PAREN_TOKEN &tok)
{
    return strm << "PAREN " << ((TOKEN) tok) << ": " << tok.paren;
}
