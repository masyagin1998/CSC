#include "ifstmtast.hpp"

IF_STMT_AST*IF_STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    IF_BRANCH_STMT_AST*if_branch = IF_BRANCH_STMT_AST::read(tok, lexer);
    if ((*tok)->get_tag() != DOMAIN_TAG::ELSE) {
        return new IF_STMT_AST(if_branch, nullptr);
    } else {
        delete (*tok);
        (*tok) = lexer->next_token();
        try {
            if ((*tok)->get_tag() == DOMAIN_TAG::IF) {
                return new IF_STMT_AST(if_branch, IF_STMT_AST::read(tok, lexer));
            } else if ((*tok)->get_tag() == DOMAIN_TAG::LBRACE) {
                return new IF_STMT_AST(if_branch, COMPOUND_STMT_AST::read(tok, lexer));
            }
        } catch (PARSER_EXCEPTION &excp) {
            excp.prepend_exception("if->");
            throw excp;
        }
        throw PARSER_EXCEPTION("unknown stmt at");
    }
}

IF_STMT_AST::IF_STMT_AST(IF_BRANCH_STMT_AST*first, AST*second) : first(first), second(second) {}

IF_BRANCH_STMT_AST*IF_STMT_AST::get_first() { return first; }
AST*IF_STMT_AST::get_second() { return second; }

IF_STMT_AST::~IF_STMT_AST()
{
    delete first;
    if (second != nullptr) {
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, IF_STMT_AST &if_stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < if_stmt.tabs; i++) {
        tabs_str += '\t';
    }

    strm << tabs_str << "IF STMT BEGIN:" << std::endl;

    // first.
    if_stmt.first->add_tab(if_stmt.tabs + 1);
    strm << *(if_stmt.first);
    if_stmt.first->del_tab(if_stmt.tabs + 1);
    // second.
    if (if_stmt.second != nullptr) {
        if_stmt.second->add_tab(if_stmt.tabs + 1);
        COMPOUND_STMT_AST*comp_stmt;
        IF_STMT_AST*else_stmt;
        if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(if_stmt.second)) != nullptr) {
            strm << *(comp_stmt);
        } else if ((else_stmt = dynamic_cast<IF_STMT_AST*>(if_stmt.second)) != nullptr) {
            strm << *(else_stmt);
        }
        if_stmt.second->del_tab(if_stmt.tabs + 1);
    }
    
    strm << tabs_str << "IF STMT END;" << std::endl;
    return strm;
}

IF_BRANCH_STMT_AST*IF_BRANCH_STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() != DOMAIN_TAG::IF) {
        throw PARSER_EXCEPTION("bad beginning of if at");
    }
    delete (*tok);
    (*tok) = lexer->next_token();
    if ((*tok)->get_tag() != DOMAIN_TAG::LPAREN) {
        throw PARSER_EXCEPTION("bad beginning of if condition at");
    }
    delete (*tok);
    (*tok) = lexer->next_token();
    LOGICAL_OR_EXPR_AST*logical;
    try {
        logical = LOGICAL_OR_EXPR_AST::read(tok, lexer);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("if->");
        throw excp;
    }
    if ((*tok)->get_tag() != DOMAIN_TAG::RPAREN) {
        throw PARSER_EXCEPTION("bad ending of if condition at");
    }
    delete (*tok);
    (*tok) = lexer->next_token();
    COMPOUND_STMT_AST*comp_stmt;
    try {
        comp_stmt = COMPOUND_STMT_AST::read(tok, lexer);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("if->");
        throw excp;
    }
    return new IF_BRANCH_STMT_AST(logical, comp_stmt);
}

IF_BRANCH_STMT_AST::IF_BRANCH_STMT_AST(LOGICAL_OR_EXPR_AST*logical, COMPOUND_STMT_AST*comp_stmt) : logical(logical), comp_stmt(comp_stmt) {}

LOGICAL_OR_EXPR_AST*IF_BRANCH_STMT_AST::get_logical() { return logical; }
COMPOUND_STMT_AST*IF_BRANCH_STMT_AST::get_comp_stmt() { return comp_stmt; }

IF_BRANCH_STMT_AST::~IF_BRANCH_STMT_AST()
{
    delete logical;
    delete comp_stmt;
}

std::ostream& operator<<(std::ostream &strm, IF_BRANCH_STMT_AST &if_branch_stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < if_branch_stmt.tabs; i++) {
        tabs_str += '\t';
    }

    strm << tabs_str << "IF BRANCH BEGIN:" << std::endl;

    // logical.
    if_branch_stmt.logical->add_tab(if_branch_stmt.tabs + 1);
    strm << *(if_branch_stmt.logical);
    if_branch_stmt.logical->del_tab(if_branch_stmt.tabs + 1);
    // comp_stmt.
    if_branch_stmt.comp_stmt->add_tab(if_branch_stmt.tabs + 1);
    strm << *(if_branch_stmt.comp_stmt);
    if_branch_stmt.comp_stmt->del_tab(if_branch_stmt.tabs + 1);
    
    strm << tabs_str << "IF BRANCH END;" << std::endl;
    return strm;
}
