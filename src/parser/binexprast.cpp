#include "binexprast.hpp"
#include "unaryexprast.hpp"

#include <iostream>

ADD_EXPR_AST*ADD_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    MULT_EXPR_AST*first = MULT_EXPR_AST::read(tok, lexer);
    if ((*tok)->get_tag() == DOMAIN_TAG::PLUS) {
        DOMAIN_TAG*add_op = new DOMAIN_TAG;
        *add_op = DOMAIN_TAG::PLUS;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new ADD_EXPR_AST(first, add_op, ADD_EXPR_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::MINUS) {
        DOMAIN_TAG*add_op = new DOMAIN_TAG;
        *add_op = DOMAIN_TAG::MINUS;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new ADD_EXPR_AST(first, add_op, ADD_EXPR_AST::read(tok, lexer));
    } else {
        return new ADD_EXPR_AST(first, nullptr, nullptr);
    }
}

ADD_EXPR_AST::ADD_EXPR_AST(MULT_EXPR_AST*first, DOMAIN_TAG*add_op, ADD_EXPR_AST*second) : first(first), add_op(add_op), second(second) {}

MULT_EXPR_AST*ADD_EXPR_AST::get_first() { return first; };
DOMAIN_TAG*ADD_EXPR_AST::get_add_op() { return add_op; }
ADD_EXPR_AST*ADD_EXPR_AST::get_second() { return second; };

ADD_EXPR_AST::~ADD_EXPR_AST()
{
    delete first;
    if (add_op != nullptr) {
        delete add_op;
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, ADD_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "ADD EXPR BEGIN:" << std::endl;
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    if (expr.add_op != nullptr) {
        // add_op.
        switch (*expr.add_op) {
        case DOMAIN_TAG::PLUS:
            strm << tabs_str << "+" << std::endl;
            break;
        case DOMAIN_TAG::MINUS:
            strm << tabs_str << "-" << std::endl;
            break;
        default:
            // do nothing.
            break;
        }
        // second.
        expr.second->add_tab(expr.tabs + 1);
        strm << *(expr.second);
        expr.second->del_tab(expr.tabs + 1);
    }
    
    strm << tabs_str << "ADD EXPR END;" << std::endl;
    return strm;
}

MULT_EXPR_AST*MULT_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    LEFT_UNARY_EXPR_AST*first = LEFT_UNARY_EXPR_AST::read(tok, lexer);
    if ((*tok)->get_tag() == DOMAIN_TAG::MUL) {
        DOMAIN_TAG*mult_op = new DOMAIN_TAG;
        *mult_op = DOMAIN_TAG::MUL;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new MULT_EXPR_AST(first, mult_op, MULT_EXPR_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::DIV) {
        DOMAIN_TAG*mult_op = new DOMAIN_TAG;
        *mult_op = DOMAIN_TAG::DIV;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new MULT_EXPR_AST(first, mult_op, MULT_EXPR_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::MOD) {
        DOMAIN_TAG*mult_op = new DOMAIN_TAG;
        *mult_op = DOMAIN_TAG::MOD;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new MULT_EXPR_AST(first, mult_op, MULT_EXPR_AST::read(tok, lexer));
    } else {
        return new MULT_EXPR_AST(first, nullptr, nullptr);
    }
}

MULT_EXPR_AST::MULT_EXPR_AST(LEFT_UNARY_EXPR_AST*first, DOMAIN_TAG*mult_op, MULT_EXPR_AST*second) : first(first), mult_op(mult_op), second(second) {}

LEFT_UNARY_EXPR_AST*MULT_EXPR_AST::get_first() { return first; }
DOMAIN_TAG*MULT_EXPR_AST::get_mult_op() { return mult_op; }
MULT_EXPR_AST*MULT_EXPR_AST::get_second() { return second; }

MULT_EXPR_AST::~MULT_EXPR_AST()
{
    delete first;
    if (mult_op != nullptr) {
        delete mult_op;
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, MULT_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "MULT EXPR BEGIN:" << std::endl;
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    if (expr.mult_op != nullptr) {
        // mult_op.
        switch (*expr.mult_op) {
        case DOMAIN_TAG::MUL:
            strm << tabs_str << "*" << std::endl;
            break;
        case DOMAIN_TAG::DIV:
            strm << tabs_str << "/" << std::endl;
            break;
        case DOMAIN_TAG::MOD:
            strm << tabs_str << "%" << std::endl;
            break;
        default:
            // do nothing.
            break;
        }
        // second.
        expr.second->add_tab(expr.tabs + 1);
        strm << *(expr.second);
        expr.second->del_tab(expr.tabs + 1);
    }
    
    strm << tabs_str << "MULT EXPR END;" << std::endl;
    return strm;
}
