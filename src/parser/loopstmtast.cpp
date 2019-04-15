#include "loopstmtast.hpp"

WHILE_STMT_AST*WHILE_STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() != DOMAIN_TAG::WHILE) {
        throw PARSER_EXCEPTION("bad beginning of while loop at");
    }
    delete (*tok);
    (*tok) = lexer->next_token();
    if ((*tok)->get_tag() != DOMAIN_TAG::LPAREN) {
        throw PARSER_EXCEPTION("bad beginning of while loop condition at");
    }
    delete (*tok);
    (*tok) = lexer->next_token();
    LOGICAL_OR_EXPR_AST*logical;
    try {
        logical = LOGICAL_OR_EXPR_AST::read(tok, lexer);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("while->");
        throw excp;
    }
    if ((*tok)->get_tag() != DOMAIN_TAG::RPAREN) {
        throw PARSER_EXCEPTION("bad ending of while loop condition at");
    }
    delete (*tok);
    (*tok) = lexer->next_token();

    try {
        COMPOUND_STMT_AST*comp_stmt = COMPOUND_STMT_AST::read(tok, lexer);
        return new WHILE_STMT_AST(logical, comp_stmt);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("while->");
        throw excp;
    }
}

WHILE_STMT_AST::WHILE_STMT_AST(LOGICAL_OR_EXPR_AST*logical, COMPOUND_STMT_AST*comp_stmt) : logical(logical), comp_stmt(comp_stmt) {}

LOGICAL_OR_EXPR_AST*WHILE_STMT_AST::get_logical() { return logical; }
COMPOUND_STMT_AST*WHILE_STMT_AST::get_comp_stmt() { return comp_stmt; }

WHILE_STMT_AST::~WHILE_STMT_AST()
{
    delete logical;
    delete comp_stmt;
}

std::ostream& operator<<(std::ostream &strm, WHILE_STMT_AST &while_stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < while_stmt.tabs; i++) {
        tabs_str += '\t';
    }

    strm << tabs_str << "WHILE_STMT BEGIN:" << std::endl;

    // logical.
    while_stmt.logical->add_tab(while_stmt.tabs + 1);
    strm << *(while_stmt.logical);
    while_stmt.logical->del_tab(while_stmt.tabs + 1);
    // compound_stmt.
    while_stmt.comp_stmt->add_tab(while_stmt.tabs + 1);
    strm << *(while_stmt.comp_stmt);
    while_stmt.comp_stmt->del_tab(while_stmt.tabs + 1);
    
    strm << tabs_str << "ASS_STMT END;" << std::endl;
    return strm;
}
