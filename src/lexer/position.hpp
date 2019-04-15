#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <ostream>

class POSITION
{
  private:
    std::string*program;
    
    std::size_t line;
    std::size_t pos;
    std::size_t index;

  public:
    POSITION(std::string*program);
    POSITION(std::string*program, std::size_t line, std::size_t pos, std::size_t index);

    int get_line() const;
    int get_pos() const;

    POSITION next() const;

    bool is_EOF() const;
    bool is_newline() const;
    bool is_whitespace() const;
    bool is_letter() const;
    bool is_decimal_digit() const;
    bool is_unknown() const;
    
    int get_code() const;

    friend std::ostream& operator<<(std::ostream &strm, const POSITION &pos);
};

#endif  // POSITION_HPP_INCLUDED
