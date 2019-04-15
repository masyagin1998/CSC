#include "assigntokens.hpp"

const std::string ASSIGN_TOKEN::ASSIGN = "=";
const std::string ASSIGN_TOKEN::ASSIGN_PLUS = "+=";
const std::string ASSIGN_TOKEN::ASSIGN_MINUS = "-=";
const std::string ASSIGN_TOKEN::ASSIGN_MUL = "*=";
const std::string ASSIGN_TOKEN::ASSIGN_DIV = "/=";
const std::string ASSIGN_TOKEN::ASSIGN_MOD = "%=";

DOMAIN_TAG ASSIGN_TOKEN::is(POSITION pos)
{
    if ((pos.get_code() == '=') && (pos.next().get_code() != '=')) {
        return DOMAIN_TAG::ASSIGN;
    } else if ((pos.get_code() == '+') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::ASSIGN_PLUS;
    } else if ((pos.get_code() == '-') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::ASSIGN_MINUS;
    } else if ((pos.get_code() == '*') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::ASSIGN_MUL;
    } else if ((pos.get_code() == '/') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::ASSIGN_DIV;
    } else if ((pos.get_code() == '%') && (pos.next().get_code() == '=')) {
        return DOMAIN_TAG::ASSIGN_MOD;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

ASSIGN_TOKEN*ASSIGN_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string assign;

    switch (tag) {
    case DOMAIN_TAG::ASSIGN:
        assign = ASSIGN;
        break;
    case DOMAIN_TAG::ASSIGN_PLUS:
        assign = ASSIGN_PLUS;
        break;
    case DOMAIN_TAG::ASSIGN_MINUS:
        assign = ASSIGN_MINUS;
        break;
    case DOMAIN_TAG::ASSIGN_MUL:
        assign = ASSIGN_MUL;
        break;
    case DOMAIN_TAG::ASSIGN_DIV:
        assign = ASSIGN_DIV;
        break;
    case DOMAIN_TAG::ASSIGN_MOD:
        assign = ASSIGN_MOD;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < assign.size()) {
        i++;
        pos = pos.next();
    }

    return new ASSIGN_TOKEN(tag, starting, pos, assign);
}

ASSIGN_TOKEN::ASSIGN_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string assign) : TOKEN(tag, starting, following), assign(assign) {}

std::ostream& operator<<(std::ostream &strm, const ASSIGN_TOKEN &tok)
{
    return strm << "ASSIGN " << ((TOKEN) tok) << ": " << tok.assign;
}
