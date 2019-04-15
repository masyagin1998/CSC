#include "identast.hpp"

IDENT_AST*IDENT_AST::read(TOKEN**tok, LEXER*lexer)
{
    switch ((*tok)->get_tag()) {
    case DOMAIN_TAG::IDENT: {
        std::string ident = (dynamic_cast<IDENT_TOKEN*>(*tok))->get_ident();
        delete *tok;
        (*tok) = lexer->next_token();
        return new IDENT_AST(ident);
        break;
    }
    default:
        throw PARSER_EXCEPTION("unknown ident at");
        break;
    }
}

IDENT_AST::IDENT_AST(std::string ident) : ident(ident) {}

std::string IDENT_AST::get_ident() const { return ident; }    
void IDENT_AST::set_ident(std::string) { this->ident = ident; }

std::ostream& operator<<(std::ostream &strm, IDENT_AST &ident)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < ident.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "IDENT: ";
    strm << ident.ident;
    strm << std::endl;
    
    return strm;
}
