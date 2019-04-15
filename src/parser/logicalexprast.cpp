#include "logicalexprast.hpp"
#include "eqexprast.hpp"

LOGICAL_OR_EXPR_AST*LOGICAL_OR_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    LOGICAL_AND_EXPR_AST*first = LOGICAL_AND_EXPR_AST::read(tok, lexer);
    if ((*tok)->get_tag() != DOMAIN_TAG::LOGICAL_OR) {
        return new LOGICAL_OR_EXPR_AST(first, nullptr);
    } else {
        delete (*tok);
        (*tok) = lexer->next_token();
        LOGICAL_OR_EXPR_AST*second = LOGICAL_OR_EXPR_AST::read(tok, lexer);
        return new LOGICAL_OR_EXPR_AST(first, second);
    }
}

LOGICAL_OR_EXPR_AST::LOGICAL_OR_EXPR_AST(LOGICAL_AND_EXPR_AST*first, LOGICAL_OR_EXPR_AST*second) : first(first), second(second) {}

LOGICAL_AND_EXPR_AST*LOGICAL_OR_EXPR_AST::get_first() { return first; };
LOGICAL_OR_EXPR_AST*LOGICAL_OR_EXPR_AST::get_second() { return second; };

LOGICAL_OR_EXPR_AST::~LOGICAL_OR_EXPR_AST()
{
    delete first;
    if (second != nullptr) {
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, LOGICAL_OR_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "LOGICAL OR EXPR BEGIN:" << std::endl;
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    if (expr.second != nullptr) {
        strm << tabs_str << "||" << std::endl;
        // second.
        expr.second->add_tab(expr.tabs + 1);
        strm << *(expr.second);
        expr.second->del_tab(expr.tabs + 1);
    }
    
    strm << tabs_str << "LOGICAL OR EXPR END;" << std::endl;
    return strm;
}

LOGICAL_AND_EXPR_AST*LOGICAL_AND_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    EQ_EXPR_AST*first = EQ_EXPR_AST::read(tok, lexer);
    if ((*tok)->get_tag() != DOMAIN_TAG::LOGICAL_AND) {
        return new LOGICAL_AND_EXPR_AST(first, nullptr);
    } else {
        delete (*tok);
        (*tok) = lexer->next_token();
        LOGICAL_AND_EXPR_AST*second = LOGICAL_AND_EXPR_AST::read(tok, lexer);
        return new LOGICAL_AND_EXPR_AST(first, second);
    }
}

LOGICAL_AND_EXPR_AST::LOGICAL_AND_EXPR_AST(EQ_EXPR_AST*first, LOGICAL_AND_EXPR_AST*second) : first(first), second(second) {}

EQ_EXPR_AST*LOGICAL_AND_EXPR_AST::get_first() { return first; };
LOGICAL_AND_EXPR_AST*LOGICAL_AND_EXPR_AST::get_second() { return second; };

LOGICAL_AND_EXPR_AST::~LOGICAL_AND_EXPR_AST()
{
    delete first;
    if (second != nullptr) {
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, LOGICAL_AND_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "LOGICAL AND EXPR BEGIN:" << std::endl;
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    if (expr.second != nullptr) {
        strm << tabs_str << "&&" << std::endl;
        // second.
        expr.second->add_tab(expr.tabs + 1);
        strm << *(expr.second);
        expr.second->del_tab(expr.tabs + 1);
    }
    
    strm << tabs_str << "LOGICAL AND EXPR END;" << std::endl;
    return strm;
}
