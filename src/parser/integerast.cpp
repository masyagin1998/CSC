#include "integerast.hpp"

INTEGER_AST*INTEGER_AST::read(TOKEN*tok, LEXER*lexer)
{
    switch (tok->get_tag()) {
    case DOMAIN_TAG::INTEGER: {
        int integer = (dynamic_cast<INTEGER_TOKEN*>(tok))->get_integer();
        delete tok;
        tok = lexer->next_token();
        return new INTEGER_AST(integer);
        break;
    }
    default:
        throw PARSER_EXCEPTION("unknown integer at");
        break;
    }
}

INTEGER_AST::INTEGER_AST(int integer) : integer(integer) {}

int INTEGER_AST::get_integer() const { return integer; }
void INTEGER_AST::set_integer(int integer) { this->integer = integer; }

std::ostream& operator<<(std::ostream &strm, INTEGER_AST &integer)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < integer.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "INTEGER: ";
    strm << integer.integer;
    strm << std::endl;
    
    return strm;
}
