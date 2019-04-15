#ifndef PARSER_EXCEPTION_HPP_INCLUDED
#define PARSER_EXCEPTION_HPP_INCLUDED

#include <string>

class PARSER_EXCEPTION
{
  private:
    std::string exception;
    
  public:
    PARSER_EXCEPTION(std::string exception);
    
    void prepend_exception(std::string prepend);

    std::string get_exception();

    friend std::ostream& operator<<(std::ostream &strm, const PARSER_EXCEPTION &excp);
};

#endif  // PARSER_EXCEPTION_HPP_INCLUDED
