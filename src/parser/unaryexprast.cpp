#include "unaryexprast.hpp"

LEFT_UNARY_EXPR_AST*LEFT_UNARY_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() == DOMAIN_TAG::PLUS) {
        DOMAIN_TAG*left_unary_op = new DOMAIN_TAG;
        *left_unary_op = DOMAIN_TAG::PLUS;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new LEFT_UNARY_EXPR_AST(left_unary_op, PRIMARY_EXPR_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::MINUS) {
        DOMAIN_TAG*left_unary_op = new DOMAIN_TAG;
        *left_unary_op = DOMAIN_TAG::MINUS;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new LEFT_UNARY_EXPR_AST(left_unary_op, PRIMARY_EXPR_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::LOGICAL_NOT) {
        DOMAIN_TAG*left_unary_op = new DOMAIN_TAG;
        *left_unary_op = DOMAIN_TAG::LOGICAL_NOT;
        delete (*tok);
        (*tok) = lexer->next_token();
        return new LEFT_UNARY_EXPR_AST(left_unary_op, PRIMARY_EXPR_AST::read(tok, lexer));
    } else {
        return new LEFT_UNARY_EXPR_AST(nullptr, PRIMARY_EXPR_AST::read(tok, lexer));
    }
}

LEFT_UNARY_EXPR_AST::LEFT_UNARY_EXPR_AST(DOMAIN_TAG*left_unary_op, PRIMARY_EXPR_AST*first) : left_unary_op(left_unary_op), first(first) {}

DOMAIN_TAG*LEFT_UNARY_EXPR_AST::get_left_unary_op() { return left_unary_op; }
PRIMARY_EXPR_AST*LEFT_UNARY_EXPR_AST::get_first() { return first; };

LEFT_UNARY_EXPR_AST::~LEFT_UNARY_EXPR_AST()
{
    delete first;
    if (left_unary_op != nullptr) {
        delete left_unary_op;
    }
}

std::ostream& operator<<(std::ostream &strm, LEFT_UNARY_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "LEFT UNARY EXPR BEGIN:" << std::endl;
    if (expr.left_unary_op != nullptr) {
        // left_unary_op.
        switch (*expr.left_unary_op) {
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
    }
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    
    strm << tabs_str << "LEFT UNARY EXPR END;" << std::endl;
    return strm;
}
