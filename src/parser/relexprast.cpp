#include "relexprast.hpp"

REL_EXPR_AST*REL_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> add_exprs;
    add_exprs.push_back(std::make_pair(ADD_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    while (((*tok)->get_tag() == DOMAIN_TAG::LESS) || ((*tok)->get_tag() == DOMAIN_TAG::LESS_OR_EQ) ||
           ((*tok)->get_tag() == DOMAIN_TAG::MORE) || ((*tok)->get_tag() == DOMAIN_TAG::MORE_OR_EQ)) {
        add_exprs.back().second = (*tok)->get_tag();
        delete (*tok);
        (*tok) = lexer->next_token();
        add_exprs.push_back(std::make_pair(ADD_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    }
    return new REL_EXPR_AST(add_exprs);
}

REL_EXPR_AST::REL_EXPR_AST(std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> add_exprs) : add_exprs(add_exprs) {}

std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> REL_EXPR_AST::get_add_exprs() { return add_exprs; }
void REL_EXPR_AST::set_add_exprs(std::vector<std::pair<ADD_EXPR_AST*, DOMAIN_TAG>> add_exprs) { this->add_exprs = add_exprs; }

REL_EXPR_AST::~REL_EXPR_AST()
{
   for (std::size_t i = 0; i < add_exprs.size(); i++) {
       delete add_exprs[i].first;
   }
}

std::ostream& operator<<(std::ostream &strm, REL_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "REL EXPR BEGIN:" << std::endl;
    expr.add_exprs[0].first->add_tab(expr.tabs + 1);
    strm << *(expr.add_exprs[0].first);
    expr.add_exprs[0].first->del_tab(expr.tabs + 1);
    for (std::size_t i = 1; i < expr.add_exprs.size(); i++) {
        if (expr.add_exprs[i - 1].second == DOMAIN_TAG::LESS) {
            strm << tabs_str << "<" << std::endl;
        } else if (expr.add_exprs[i - 1].second == DOMAIN_TAG::LESS_OR_EQ) {
            strm << tabs_str << "<=" << std::endl;
        } else if (expr.add_exprs[i - 1].second == DOMAIN_TAG::MORE) {
            strm << tabs_str << ">" << std::endl;
        } else if (expr.add_exprs[i - 1].second == DOMAIN_TAG::MORE_OR_EQ) {
            strm << tabs_str << ">=" << std::endl;
        }
        expr.add_exprs[i].first->add_tab(expr.tabs + 1);
        strm << *(expr.add_exprs[i].first);
        expr.add_exprs[i].first->del_tab(expr.tabs + 1);
    }
    strm << tabs_str << "REL EXPR END;" << std::endl;
    return strm;
}
