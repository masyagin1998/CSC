#include "token.hpp"

TOKEN::TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following) : tag(tag), frag(FRAGMENT(starting, following)) {}

DOMAIN_TAG TOKEN::get_tag() const
{
    return tag;
}

FRAGMENT TOKEN::get_frag() const
{
    return frag;
}

std::ostream& operator<<(std::ostream &strm, const TOKEN &tok)
{
    return strm << tok.frag;
}
