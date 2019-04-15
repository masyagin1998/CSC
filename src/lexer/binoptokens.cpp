#include "binoptokens.hpp"

const std::string BIN_OP_TOKEN::BIN_OP_PLUS = "+";
const std::string BIN_OP_TOKEN::BIN_OP_MINUS = "-";
const std::string BIN_OP_TOKEN::BIN_OP_MUL = "*";
const std::string BIN_OP_TOKEN::BIN_OP_DIV = "/";
const std::string BIN_OP_TOKEN::BIN_OP_MOD = "%";

DOMAIN_TAG BIN_OP_TOKEN::is(POSITION pos)
{
    if (pos.get_code() == '+') {
        return DOMAIN_TAG::PLUS;
    } else if (pos.get_code() == '-') {
        return DOMAIN_TAG::MINUS;
    } else if (pos.get_code() == '*') {
        return DOMAIN_TAG::MUL;
    } else if (pos.get_code() == '/') {
        return DOMAIN_TAG::DIV;
    } else if (pos.get_code() == '%') {
        return DOMAIN_TAG::MOD;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

BIN_OP_TOKEN*BIN_OP_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string bin_op;

    switch (tag) {
    case DOMAIN_TAG::PLUS:
        bin_op = BIN_OP_PLUS;
        break;
    case DOMAIN_TAG::MINUS:
        bin_op = BIN_OP_MINUS;
        break;
    case DOMAIN_TAG::MUL:
        bin_op = BIN_OP_MUL;
        break;
    case DOMAIN_TAG::DIV:
        bin_op = BIN_OP_DIV;
        break;
    case DOMAIN_TAG::MOD:
        bin_op = BIN_OP_MOD;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < bin_op.size()) {
        i++;
        pos = pos.next();
    }

    return new BIN_OP_TOKEN(tag, starting, pos, bin_op);
}

BIN_OP_TOKEN::BIN_OP_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string bin_op) : TOKEN(tag, starting, following), bin_op(bin_op) {}

std::ostream& operator<<(std::ostream &strm, const BIN_OP_TOKEN &tok)
{
    return strm << "BIN_OP " << ((TOKEN) tok) << ": " << tok.bin_op;
}
