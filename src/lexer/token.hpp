#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include "domaintag.hpp"
#include "fragment.hpp"

#include <string>
#include <ostream>

class TOKEN
{
  private:
    DOMAIN_TAG tag;
    FRAGMENT   frag;

  public:
    DOMAIN_TAG get_tag() const;
    FRAGMENT   get_frag() const;

    virtual ~TOKEN()=default;

  protected:
    TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following);

    friend std::ostream& operator<<(std::ostream &strm, const TOKEN &tok);
};

#endif  // TOKEN_HPP_INCLUDED
