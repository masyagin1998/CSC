#ifndef FRAGMENT_HPP_INCLUDED
#define FRAGMENT_HPP_INCLUDED

#include "position.hpp"

#include <ostream>

class FRAGMENT
{
  private:
    POSITION starting, following;

  public:
    FRAGMENT(POSITION starting, POSITION following);

    POSITION get_starting() const;
    POSITION get_following() const;

    friend std::ostream& operator<<(std::ostream &strm, const FRAGMENT &frag);
};

#endif  // FRAGMENT_HPP_INCLUDED
