#include "eqexprast.hpp"

EQ_EXPR_AST*EQ_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    REL_EXPR_AST*first = REL_EXPR_AST::read(tok, lexer);
    if ((*tok)->get_tag() == DOMAIN_TAG::EQ) {
        delete (*tok);
        (*tok) = lexer->next_token();
        DOMAIN_TAG*neq = new DOMAIN_TAG;
        *neq = DOMAIN_TAG::EQ;
        return new EQ_EXPR_AST(first, neq, EQ_EXPR_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::NEQ) {
        delete (*tok);
        (*tok) = lexer->next_token();
        DOMAIN_TAG*eq = new DOMAIN_TAG;
        *eq = DOMAIN_TAG::NEQ;
        return new EQ_EXPR_AST(first, eq, EQ_EXPR_AST::read(tok, lexer));
    } else {
        return new EQ_EXPR_AST(first, nullptr, nullptr);
    }
}

EQ_EXPR_AST::EQ_EXPR_AST(REL_EXPR_AST*first, DOMAIN_TAG*eq_op, EQ_EXPR_AST*second) : first(first), eq_op(eq_op), second(second) {}

REL_EXPR_AST*EQ_EXPR_AST::get_first() { return first; };
DOMAIN_TAG*EQ_EXPR_AST::get_eq_op() { return eq_op; }
EQ_EXPR_AST*EQ_EXPR_AST::get_second() { return second; };

EQ_EXPR_AST::~EQ_EXPR_AST()
{
    delete first;
    if (eq_op != nullptr) {
        delete eq_op;
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, EQ_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "EQ EXPR BEGIN:" << std::endl;
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    if (expr.eq_op != nullptr) {
        // eq_op.
        switch (*expr.eq_op) {
        case DOMAIN_TAG::EQ:
            strm << tabs_str << "==" << std::endl;
            break;
        case DOMAIN_TAG::NEQ:
            strm << tabs_str << "!=" << std::endl;
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
    
    strm << tabs_str << "EQ EXPR END;" << std::endl;
    return strm;
}
