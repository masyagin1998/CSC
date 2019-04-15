#include "declast.hpp"

DECL_AST*DECL_AST::read(TOKEN**tok, LEXER*lexer)
{
    DATATYPE_AST*datatype;
    IDENT_AST*ident;

    try {
        datatype = DATATYPE_AST::read(tok, lexer);
        ident = IDENT_AST::read(tok, lexer);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("decl->");
        throw excp;
    }

    if ((*tok)->get_tag() == DOMAIN_TAG::SEMICOLON) {
        delete (*tok);
        (*tok) = lexer->next_token();
        return new DECL_AST(datatype, ident, nullptr);
    } else if ((*tok)->get_tag() == DOMAIN_TAG::ASSIGN) {
        delete (*tok);
        (*tok) = lexer->next_token();
        
        LOGICAL_OR_EXPR_AST*expr;
        try {
            expr = LOGICAL_OR_EXPR_AST::read(tok, lexer);
        } catch (PARSER_EXCEPTION &excp) {
            excp.prepend_exception("decl->");
            throw excp;
        }
        if ((*tok)->get_tag() != DOMAIN_TAG::SEMICOLON) {
            throw PARSER_EXCEPTION("bad ending of decl at");
        }
        delete (*tok);
        (*tok) = lexer->next_token();
        return new DECL_AST(datatype, ident, expr);
    }

    throw PARSER_EXCEPTION("bad ending of decl at");
}

DECL_AST::DECL_AST(DATATYPE_AST*datatype,
                   IDENT_AST*ident,
                   LOGICAL_OR_EXPR_AST*expr) :
    datatype(datatype),
    ident(ident),
    expr(expr) {}

DATATYPE_AST*DECL_AST::get_datatype() { return datatype; }
IDENT_AST*DECL_AST::get_ident() { return ident; };
LOGICAL_OR_EXPR_AST* DECL_AST::get_expr() { return expr; }

std::ostream& operator<<(std::ostream &strm, DECL_AST &decl)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < decl.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "DECL BEGIN:" << std::endl;
    // datatype.
    decl.datatype->add_tab(decl.tabs + 1);
    strm << *(decl.datatype);
    decl.datatype->del_tab(decl.tabs + 1);
    // ident.
    decl.ident->add_tab(decl.tabs + 1);
    strm << *(decl.ident);
    decl.ident->del_tab(decl.tabs + 1);
    // expr.
    if (decl.expr != nullptr) {
        decl.expr->add_tab(decl.tabs + 1);
        strm << *(decl.expr);
        decl.expr->del_tab(decl.tabs + 1);
    }
    
    strm << tabs_str << "DECL END;" << std::endl;
    return strm;
}
