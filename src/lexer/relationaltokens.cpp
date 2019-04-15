#include "relationaltokens.hpp"

const std::string RELATIONAL_TOKEN::MORE = ">";
const std::string RELATIONAL_TOKEN::LESS = "<";
const std::string RELATIONAL_TOKEN::MORE_OR_EQ = ">=";
const std::string RELATIONAL_TOKEN::LESS_OR_EQ = "<=";

DOMAIN_TAG RELATIONAL_TOKEN::is(POSITION pos)
{
    if ((pos.get_code() == '>') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::MORE_OR_EQ;
    } else if (pos.get_code() == '>') {
        return DOMAIN_TAG::MORE;
    } else if ((pos.get_code() == '<') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::LESS_OR_EQ;
    } else if (pos.get_code() == '<') {
        return DOMAIN_TAG::LESS;
    } else {
        // TODO: throw exception.
    }

    return DOMAIN_TAG::END_OF_FILE;
}

RELATIONAL_TOKEN*RELATIONAL_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string relational;

    switch (tag) {
    case DOMAIN_TAG::MORE:
        relational = MORE;
        break;
    case DOMAIN_TAG::LESS:
        relational = LESS;
        break;
    case DOMAIN_TAG::MORE_OR_EQ:
        relational = MORE_OR_EQ;
        break;
    case DOMAIN_TAG::LESS_OR_EQ:
        relational = LESS_OR_EQ;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < relational.size()) {
        i++;
        pos = pos.next();
    }

    return new RELATIONAL_TOKEN(tag, starting, pos, relational);
}

RELATIONAL_TOKEN::RELATIONAL_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string relational) : TOKEN(tag, starting, following), relational(relational) {}

std::ostream& operator<<(std::ostream &strm, const RELATIONAL_TOKEN &tok)
{
    return strm << "RELATIONAL " << ((TOKEN) tok) << ": " << tok.relational;
}
