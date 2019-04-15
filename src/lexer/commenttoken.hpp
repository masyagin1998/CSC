#ifndef COMMENT_TOKEN_HPP_INCLUDED
#define COMMENT_TOKEN_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class COMMENT_TOKEN : public TOKEN
{
private:
    std::string comment;
    
public:
    static DOMAIN_TAG is(POSITION pos);
    
    static COMMENT_TOKEN*read(POSITION pos, DOMAIN_TAG tag);

    COMMENT_TOKEN(std::string comment, POSITION starting, POSITION following);

    std::string get_comment() const;

    friend std::ostream& operator<<(std::ostream &strm, const COMMENT_TOKEN &tok);
};

#endif  // COMMENT_TOKEN_HPP_INCLUDED
