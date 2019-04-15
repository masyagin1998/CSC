#include "position.hpp"

POSITION::POSITION(std::string*program) : program(program), line(1), pos(1), index(0) {}

POSITION::POSITION(std::string*program, std::size_t line, std::size_t pos, std::size_t index) : program(program), line(line), pos(pos), index(index) {}

int POSITION::get_line() const
{
    return line;
}

int POSITION::get_pos() const
{
    return pos;
}

POSITION POSITION::next() const
{
    std::size_t new_line = line;
    std::size_t new_pos = pos;
    std::size_t new_index = index;

    if (!this->is_EOF()) {
        if (this->is_newline()) {
            new_line++;
            new_pos = 1;
        } else {
            new_pos++;
        }
        new_index++;
    }

    return POSITION(program, new_line, new_pos, new_index);
}

bool POSITION::is_EOF() const
{
    return index == (*program).size() - 1;
}

bool POSITION::is_newline() const
{
    int ch = (*program)[index];
    return (ch == '\n');
}

bool POSITION::is_whitespace() const
{
    int ch = (*program)[index];
    if ((ch == ' ') || (ch == '\t')) return true;
    return false;
}

bool POSITION::is_letter() const
{
    int ch = (*program)[index];
    if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) return true;
    return false;
}

bool POSITION::is_decimal_digit() const
{
    int ch = (*program)[index];
    if ((ch >= '0') && (ch <= '9')) return true;
    return false;
}

bool POSITION::is_unknown() const
{
    int ch = (*program)[index];
    bool is_known = (this->is_letter() || this->is_decimal_digit() ||
                     (ch == ' ') || (ch == '\t') || (ch == '\n') ||
                     (ch == '{') || (ch == '}') ||
                     (ch == '(') || (ch == ')') ||
                     (ch == ',') || (ch == ';') ||
                     (ch == '*') || (ch == '/') || (ch == '+') || (ch == '-') || (ch == '%'));

    return (!is_known);
}

int POSITION::get_code() const
{
    int ch = (*program)[index];
    return ch;
}

std::ostream& operator<<(std::ostream &strm, const POSITION &pos)
{
    return strm << "(" << pos.line << " : " << pos.pos << ")";
}

