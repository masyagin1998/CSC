#include "commenttoken.hpp"

DOMAIN_TAG COMMENT_TOKEN::is(POSITION pos)
{
    return (((pos.get_code() == '/') && (pos.next().get_code() == '/')) ||
            ((pos.get_code() == '/') && (pos.next().get_code() == '*'))) ? DOMAIN_TAG::COMMENT : DOMAIN_TAG::END_OF_FILE;
}

COMMENT_TOKEN*COMMENT_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string comment = "";

    if (tag != DOMAIN_TAG::COMMENT) {
        // TODO: throw exception.
    }

    if ((pos.get_code() == '/') && (pos.next().get_code() == '/')) {
        pos = pos.next().next();
        while (!pos.is_newline()) {
            comment += pos.get_code();
            pos = pos.next();
        }
        pos = pos.next();
    } else if ((pos.get_code() == '/') && (pos.next().get_code() == '*')) {
        pos = pos.next().next();
        while (!(pos.is_EOF() || ((pos.get_code() == '*') && (pos.next().get_code() == '/')))) {
            comment += pos.get_code();
            pos = pos.next();
        }
        pos = pos.next().next();
    } else {
        // TODO: throw exception.
    }
    
    return new COMMENT_TOKEN(comment, starting, pos);
}

COMMENT_TOKEN::COMMENT_TOKEN(std::string comment, POSITION starting, POSITION following) : TOKEN(DOMAIN_TAG::COMMENT, starting, following), comment(comment) {}

std::string COMMENT_TOKEN::get_comment() const
{
    return comment;
}

std::ostream& operator<<(std::ostream &strm, const COMMENT_TOKEN &tok)
{
    return strm << "COMMENT " << ((TOKEN) tok) << ": " << tok.comment;
}
