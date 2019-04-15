#include "argast.hpp"

ARG_AST*ARG_AST::read(TOKEN**tok, LEXER*lexer)
{
    try {
        DATATYPE_AST*datatype = DATATYPE_AST::read(tok, lexer);
        IDENT_AST*ident = IDENT_AST::read(tok, lexer);
        return new ARG_AST(datatype, ident);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("arg_ast->");
        throw excp;
    }
}

ARG_AST::ARG_AST(DATATYPE_AST*datatype,
                   IDENT_AST*ident) :
    datatype(datatype),
    ident(ident) {}

DATATYPE_AST*ARG_AST::get_datatype() { return datatype; }
IDENT_AST*ARG_AST::get_ident() { return ident; };

ARG_AST::~ARG_AST()
{
    delete datatype;
    delete ident;
}

std::ostream& operator<<(std::ostream &strm, ARG_AST &arg)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < arg.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "ARG BEGIN:" << std::endl;
    // datatype.
    arg.datatype->add_tab(arg.tabs + 1);
    strm << *(arg.datatype);
    arg.datatype->del_tab(arg.tabs + 1);
    // ident.
    arg.ident->add_tab(arg.tabs + 1);
    strm << *(arg.ident);
    arg.ident->del_tab(arg.tabs + 1);
    strm << tabs_str << "ARG END;" << std::endl;
    return strm;
}
