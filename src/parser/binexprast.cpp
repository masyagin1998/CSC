#include "binexprast.hpp"
#include "unaryexprast.hpp"

#include <iostream>

ADD_EXPR_AST*ADD_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> mult_exprs;
    mult_exprs.push_back(std::make_pair(MULT_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    while (((*tok)->get_tag() == DOMAIN_TAG::PLUS) || ((*tok)->get_tag() == DOMAIN_TAG::MINUS)) {
        mult_exprs.back().second = (*tok)->get_tag();
        delete (*tok);
        (*tok) = lexer->next_token();
        mult_exprs.push_back(std::make_pair(MULT_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    }
    return new ADD_EXPR_AST(mult_exprs);
}

ADD_EXPR_AST::ADD_EXPR_AST(std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> mult_exprs) : mult_exprs(mult_exprs) {}

std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> ADD_EXPR_AST::get_mult_exprs() { return mult_exprs; }
void ADD_EXPR_AST::set_mult_exprs(std::vector<std::pair<MULT_EXPR_AST*, DOMAIN_TAG>> mult_exprs) { this->mult_exprs = mult_exprs; }

ADD_EXPR_AST::~ADD_EXPR_AST()
{
   for (std::size_t i = 0; i < mult_exprs.size(); i++) {
       delete mult_exprs[i].first;
   }
}

std::ostream& operator<<(std::ostream &strm, ADD_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "ADD EXPR BEGIN:" << std::endl;
    expr.mult_exprs[0].first->add_tab(expr.tabs + 1);
    strm << *(expr.mult_exprs[0].first);
    expr.mult_exprs[0].first->del_tab(expr.tabs + 1);
    for (std::size_t i = 1; i < expr.mult_exprs.size(); i++) {
        if (expr.mult_exprs[i - 1].second == DOMAIN_TAG::PLUS) {
            strm << tabs_str << "+" << std::endl;
        } else if (expr.mult_exprs[i - 1].second == DOMAIN_TAG::MINUS) {
            strm << tabs_str << "-" << std::endl;
        }
        expr.mult_exprs[i].first->add_tab(expr.tabs + 1);
        strm << *(expr.mult_exprs[i].first);
        expr.mult_exprs[i].first->del_tab(expr.tabs + 1);
    }
    strm << tabs_str << "ADD EXPR END;" << std::endl;
    return strm;
}

MULT_EXPR_AST*MULT_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> left_unary_exprs;
    left_unary_exprs.push_back(std::make_pair(LEFT_UNARY_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    while (((*tok)->get_tag() == DOMAIN_TAG::MUL) ||
           ((*tok)->get_tag() == DOMAIN_TAG::DIV) ||
           ((*tok)->get_tag() == DOMAIN_TAG::MOD)) {
        left_unary_exprs.back().second = (*tok)->get_tag();
        delete (*tok);
        (*tok) = lexer->next_token();
        left_unary_exprs.push_back(std::make_pair(LEFT_UNARY_EXPR_AST::read(tok, lexer), DOMAIN_TAG::END_OF_FILE));
    }
    return new MULT_EXPR_AST(left_unary_exprs);
}

MULT_EXPR_AST::MULT_EXPR_AST(std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> left_unary_exprs) : left_unary_exprs(left_unary_exprs) {}

std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> MULT_EXPR_AST::get_left_unary_exprs() { return left_unary_exprs; }
void MULT_EXPR_AST::set_left_unary_exprs(std::vector<std::pair<LEFT_UNARY_EXPR_AST*, DOMAIN_TAG>> left_unary_exprs) { this->left_unary_exprs = left_unary_exprs; }

MULT_EXPR_AST::~MULT_EXPR_AST()
{
   for (std::size_t i = 0; i < left_unary_exprs.size(); i++) {
       delete left_unary_exprs[i].first;
   }
}

std::ostream& operator<<(std::ostream &strm, MULT_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "MULT EXPR BEGIN:" << std::endl;
    expr.left_unary_exprs[0].first->add_tab(expr.tabs + 1);
    strm << *(expr.left_unary_exprs[0].first);
    expr.left_unary_exprs[0].first->del_tab(expr.tabs + 1);
    for (std::size_t i = 1; i < expr.left_unary_exprs.size(); i++) {
        if (expr.left_unary_exprs[i - 1].second == DOMAIN_TAG::MUL) {
            strm << tabs_str << "*" << std::endl;
        } else if (expr.left_unary_exprs[i - 1].second == DOMAIN_TAG::DIV) {
            strm << tabs_str << "/" << std::endl;
        } else if (expr.left_unary_exprs[i - 1].second == DOMAIN_TAG::MOD) {
            strm << tabs_str << "%" << std::endl;
        }
        expr.left_unary_exprs[i].first->add_tab(expr.tabs + 1);
        strm << *(expr.left_unary_exprs[i].first);
        expr.left_unary_exprs[i].first->del_tab(expr.tabs + 1);
    }
    strm << tabs_str << "MULT EXPR END;" << std::endl;
    return strm;
}
