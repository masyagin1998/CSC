#include "datatypeast.hpp"

DATATYPE_AST*DATATYPE_AST::read(TOKEN**tok, LEXER*lexer)
{
    // For simplicity only primitive type are allowed.
    switch ((*tok)->get_tag()) {
    case DOMAIN_TAG::INT:
        try {
            delete *tok;
            (*tok) = lexer->next_token();
            return new DATATYPE_AST(DOMAIN_TAG::INT, nullptr);
            break;
        } catch (PARSER_EXCEPTION &excp) {
            excp.prepend_exception("datatype->");
            throw excp;
        }
    default:
        throw PARSER_EXCEPTION("unknown datatype at ");
        break;
    }
}

DATATYPE_AST::DATATYPE_AST(DOMAIN_TAG basic_datatype,
                           IDENT_AST*ident) :
    basic_datatype(basic_datatype),
    ident(ident) {}

DOMAIN_TAG DATATYPE_AST::get_basic_datatype() const { return basic_datatype; }
void DATATYPE_AST::set_basic_datatype(DOMAIN_TAG basic_datatype) { this->basic_datatype = basic_datatype; }
    
IDENT_AST*DATATYPE_AST::get_ident() { return ident; }

DATATYPE_AST::~DATATYPE_AST()
{
    if (ident != nullptr) {
        delete ident;
    }
}

std::ostream& operator<<(std::ostream &strm, DATATYPE_AST &datatype)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < datatype.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "DATATYPE BEGIN:" << std::endl;
    // basic_datatype.
    strm << tabs_str << "\t" << "BASIC_DATATYPE: ";
    switch (datatype.basic_datatype) {
    case DOMAIN_TAG::INT:
        strm << "int";
        break;
    default:
        // TODO: throw exception.
        break;
    }
    strm << std::endl;
    // ident.
    if (datatype.ident != nullptr) {
        datatype.ident->add_tab(datatype.tabs + 1);
        strm << *(datatype.ident);
        datatype.ident->del_tab(datatype.tabs + 1);
    }
    strm << tabs_str << "DATATYPE END;" << std::endl;
    return strm;
}
