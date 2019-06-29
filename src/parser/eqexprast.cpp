#include "eqexprast.hpp"

EQ_EXPR_AST*EQ_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> rel_exprs;
    rel_exprs.push_back(std::make_pair(REL_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    while (((*tok)->get_tag() == DOMAIN_TAG::EQ) || ((*tok)->get_tag() == DOMAIN_TAG::NEQ)) {
        rel_exprs.back().second = (*tok)->get_tag();
        delete (*tok);
        (*tok) = lexer->next_token();
        rel_exprs.push_back(std::make_pair(REL_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    }
    return new EQ_EXPR_AST(rel_exprs);
}

EQ_EXPR_AST::EQ_EXPR_AST(std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> rel_exprs) : rel_exprs(rel_exprs) {}

std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> EQ_EXPR_AST::get_rel_exprs() { return rel_exprs; }
void EQ_EXPR_AST::set_rel_exprs(std::vector<std::pair<REL_EXPR_AST*, DOMAIN_TAG>> rel_exprs) { this->rel_exprs = rel_exprs; }

EQ_EXPR_AST::~EQ_EXPR_AST()
{
   for (std::size_t i = 0; i < rel_exprs.size(); i++) {
       delete rel_exprs[i].first;
   }
}

std::ostream& operator<<(std::ostream &strm, EQ_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "EQ EXPR BEGIN:" << std::endl;
    expr.rel_exprs[0].first->add_tab(expr.tabs + 1);
    strm << *(expr.rel_exprs[0].first);
    expr.rel_exprs[0].first->del_tab(expr.tabs + 1);
    for (std::size_t i = 1; i < expr.rel_exprs.size(); i++) {
        if (expr.rel_exprs[i - 1].second == DOMAIN_TAG::EQ) {
            strm << tabs_str << "==" << std::endl;
        } else if (expr.rel_exprs[i - 1].second == DOMAIN_TAG::NEQ) {
            strm << tabs_str << "!=" << std::endl;
        }
        expr.rel_exprs[i].first->add_tab(expr.tabs + 1);
        strm << *(expr.rel_exprs[i].first);
        expr.rel_exprs[i].first->del_tab(expr.tabs + 1);
    }
    strm << tabs_str << "EQ EXPR END;" << std::endl;
    return strm;
}
