#include "fragment.hpp"

FRAGMENT::FRAGMENT(POSITION starting, POSITION following) : starting(starting), following(following) {}

POSITION FRAGMENT::get_starting() const
{
    return this->starting;
}

POSITION FRAGMENT::get_following() const
{
    return this->following;
}

std::ostream& operator<<(std::ostream &strm, const FRAGMENT &frag)
{
    return strm << frag.starting << "-" << frag.following;
}
