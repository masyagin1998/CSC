#include "logicaltokens.hpp"

const std::string LOGICAL_TOKEN::LOGICAL_AND = "&&";
const std::string LOGICAL_TOKEN::LOGICAL_OR = "||";
const std::string LOGICAL_TOKEN::LOGICAL_NOT = "!";

DOMAIN_TAG LOGICAL_TOKEN::is(POSITION pos)
{
    if ((pos.get_code() == '&') && (pos.next().get_code() == '&')) {
        return DOMAIN_TAG::LOGICAL_AND;
    } else if ((pos.get_code() == '|') && (pos.next().get_code() == '|')) {
        return DOMAIN_TAG::LOGICAL_OR;
    } else if (pos.get_code() == '!') {
        return DOMAIN_TAG::LOGICAL_NOT;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

LOGICAL_TOKEN*LOGICAL_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string logical;

    switch (tag) {
    case DOMAIN_TAG::LOGICAL_AND:
        logical = LOGICAL_AND;
        break;
    case DOMAIN_TAG::LOGICAL_OR:
        logical = LOGICAL_OR;
        break;
    case DOMAIN_TAG::LOGICAL_NOT:
        logical = LOGICAL_NOT;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < logical.size()) {
        i++;
        pos = pos.next();
    }

    return new LOGICAL_TOKEN(tag, starting, pos, logical);
}

LOGICAL_TOKEN::LOGICAL_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string logical) : TOKEN(tag, starting, following), logical(logical) {}

std::ostream& operator<<(std::ostream &strm, const LOGICAL_TOKEN &tok)
{
    return strm << "LOGICAL " << ((TOKEN) tok) << ": " << tok.logical;
}
