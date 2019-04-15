#include "keywordtokens.hpp"

const std::string KEYWORD_TOKEN::RETURN = "return";
const std::string KEYWORD_TOKEN::FOR = "for";
const std::string KEYWORD_TOKEN::WHILE = "while";
const std::string KEYWORD_TOKEN::DO = "do";
const std::string KEYWORD_TOKEN::IF = "if";
const std::string KEYWORD_TOKEN::ELSE = "else";

static bool check_keyword(std::string keyword, POSITION pos)
{
    std::size_t i = 0;
    while (i != keyword.size()) {
        if (keyword[i] != pos.get_code()) {
            return false;
        }
        i++;
        pos = pos.next();
    }
    
    if (!pos.is_letter() && !pos.is_decimal_digit()) {
        return true;
    }

    return false;
}

DOMAIN_TAG KEYWORD_TOKEN::is(POSITION pos)
{
    if (check_keyword(RETURN, pos)) {
        return DOMAIN_TAG::RETURN;
    } else if (check_keyword(FOR, pos)) {
        return DOMAIN_TAG::FOR;
    } else if (check_keyword(WHILE, pos)) {
        return DOMAIN_TAG::WHILE;
    } else if (check_keyword(DO, pos)) {
        return DOMAIN_TAG::DO;
    } else if (check_keyword(IF, pos)) {
        return DOMAIN_TAG::IF;
    } else if (check_keyword(ELSE, pos)) {
        return DOMAIN_TAG::ELSE;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

KEYWORD_TOKEN*KEYWORD_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string keyword;
    
    switch (tag) {
    case DOMAIN_TAG::RETURN:
        keyword = RETURN;
        break;
    case DOMAIN_TAG::FOR:
        keyword = FOR;
        break;
    case DOMAIN_TAG::WHILE:
        keyword = WHILE;
        break;
    case DOMAIN_TAG::DO:
        keyword = DO;
        break;
    case DOMAIN_TAG::IF:
        keyword = IF;
        break;
    case DOMAIN_TAG::ELSE:
        keyword = ELSE;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < keyword.size()) {
        i++;
        pos = pos.next();
    }
    
    return new KEYWORD_TOKEN(tag, starting, pos, keyword);
}

KEYWORD_TOKEN::KEYWORD_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string keyword) : TOKEN(tag, starting, following), keyword(keyword) {}

std::ostream& operator<<(std::ostream &strm, const KEYWORD_TOKEN &tok)
{
    return strm << "KEYWORD " << ((TOKEN) tok) << ": " << tok.keyword;
}
