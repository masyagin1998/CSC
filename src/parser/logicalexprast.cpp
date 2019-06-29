#include "logicalexprast.hpp"
#include "eqexprast.hpp"

LOGICAL_OR_EXPR_AST*LOGICAL_OR_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    std::vector<LOGICAL_AND_EXPR_AST*> and_exprs;
    and_exprs.push_back(LOGICAL_AND_EXPR_AST::read(tok, lexer));
    while ((*tok)->get_tag() == DOMAIN_TAG::LOGICAL_OR) {
        delete (*tok);
        (*tok) = lexer->next_token();
        and_exprs.push_back(LOGICAL_AND_EXPR_AST::read(tok, lexer));
    }
    return new LOGICAL_OR_EXPR_AST(and_exprs);
}

LOGICAL_OR_EXPR_AST::LOGICAL_OR_EXPR_AST(std::vector<LOGICAL_AND_EXPR_AST*> and_exprs) : and_exprs(and_exprs) {}

std::vector<LOGICAL_AND_EXPR_AST*> LOGICAL_OR_EXPR_AST::get_and_exprs() { return and_exprs; }
void LOGICAL_OR_EXPR_AST::set_and_exprs(std::vector<LOGICAL_AND_EXPR_AST*> and_exprs) { this->and_exprs = and_exprs; }

LOGICAL_OR_EXPR_AST::~LOGICAL_OR_EXPR_AST()
{
    for (std::size_t i = 0; i < and_exprs.size(); i++) {
        delete and_exprs[i];
    }
}

std::ostream& operator<<(std::ostream &strm, LOGICAL_OR_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "LOGICAL OR EXPR BEGIN:" << std::endl;
    expr.and_exprs[0]->add_tab(expr.tabs + 1);
    strm << *(expr.and_exprs[0]);
    expr.and_exprs[0]->del_tab(expr.tabs + 1);
    for (std::size_t i = 1; i < expr.and_exprs.size(); i++) {
        strm << tabs_str << "||" << std::endl;
        expr.and_exprs[i]->add_tab(expr.tabs + 1);
        strm << *(expr.and_exprs[i]);
        expr.and_exprs[i]->del_tab(expr.tabs + 1);
    }
    strm << tabs_str << "LOGICAL OR EXPR END;" << std::endl;
    return strm;
}

LOGICAL_AND_EXPR_AST*LOGICAL_AND_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    std::vector<EQ_EXPR_AST*> eq_exprs;
    eq_exprs.push_back(EQ_EXPR_AST::read(tok, lexer));
    while ((*tok)->get_tag() == DOMAIN_TAG::LOGICAL_AND) {
        delete (*tok);
        (*tok) = lexer->next_token();
        eq_exprs.push_back(EQ_EXPR_AST::read(tok, lexer));
    }
    return new LOGICAL_AND_EXPR_AST(eq_exprs);
}

LOGICAL_AND_EXPR_AST::LOGICAL_AND_EXPR_AST(std::vector<EQ_EXPR_AST*> eq_exprs) : eq_exprs(eq_exprs) {}

std::vector<EQ_EXPR_AST*> LOGICAL_AND_EXPR_AST::get_eq_exprs() { return eq_exprs; }
void LOGICAL_AND_EXPR_AST::set_eq_exprs(std::vector<EQ_EXPR_AST*> eq_exprs) { this->eq_exprs = eq_exprs; }

LOGICAL_AND_EXPR_AST::~LOGICAL_AND_EXPR_AST()
{
    for (std::size_t i = 0; i < eq_exprs.size(); i++) {
        delete eq_exprs[i];
    }
}

std::ostream& operator<<(std::ostream &strm, LOGICAL_AND_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "LOGICAL AND EXPR BEGIN:" << std::endl;
    expr.eq_exprs[0]->add_tab(expr.tabs + 1);
    strm << *(expr.eq_exprs[0]);
    expr.eq_exprs[0]->del_tab(expr.tabs + 1);
    for (std::size_t i = 1; i < expr.eq_exprs.size(); i++) {
        strm << tabs_str << "&&" << std::endl;
        expr.eq_exprs[i]->add_tab(expr.tabs + 1);
        strm << *(expr.eq_exprs[i]);
        expr.eq_exprs[i]->del_tab(expr.tabs + 1);
    }
    strm << tabs_str << "LOGICAL AND EXPR END;" << std::endl;
    return strm;
}
