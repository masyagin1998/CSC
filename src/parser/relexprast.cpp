#include "relexprast.hpp"

REL_EXPR_AST*REL_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    ADD_EXPR_AST*first = ADD_EXPR_AST::read(tok, lexer);
    RELATIONAL_TOKEN*rel_tok;
    if ((rel_tok = dynamic_cast<RELATIONAL_TOKEN*>(*tok)) != nullptr) {
        DOMAIN_TAG*rel_op = new DOMAIN_TAG;
        *rel_op = rel_tok->get_tag();
        delete (*tok);
        (*tok) = lexer->next_token();
        REL_EXPR_AST*second = REL_EXPR_AST::read(tok, lexer);
        return new REL_EXPR_AST(first, rel_op, second);
    } else {
        return new REL_EXPR_AST(first, nullptr, nullptr);
    }
}

REL_EXPR_AST::    REL_EXPR_AST(ADD_EXPR_AST*first, DOMAIN_TAG*rel_op, REL_EXPR_AST*second) : first(first), rel_op(rel_op), second(second) {}

ADD_EXPR_AST*REL_EXPR_AST::get_first() { return first; };
DOMAIN_TAG*REL_EXPR_AST::get_rel_op() { return rel_op; }
REL_EXPR_AST*REL_EXPR_AST::get_second() { return second; };

REL_EXPR_AST::~REL_EXPR_AST()
{
    delete first;
    if (rel_op != nullptr) {
        delete rel_op;
        delete second;
    }
}

std::ostream& operator<<(std::ostream &strm, REL_EXPR_AST &expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "REL EXPR BEGIN:" << std::endl;
    // first.
    expr.first->add_tab(expr.tabs + 1);
    strm << *(expr.first);
    expr.first->del_tab(expr.tabs + 1);
    if (expr.rel_op != nullptr) {
        // eq_op.
        switch (*expr.rel_op) {
        case DOMAIN_TAG::LESS:
            strm << tabs_str << "<" << std::endl;
            break;
        case DOMAIN_TAG::MORE:
            strm << tabs_str << ">" << std::endl;
            break;
        case DOMAIN_TAG::LESS_OR_EQ:
            strm << tabs_str << "<=" << std::endl;
            break;
        case DOMAIN_TAG::MORE_OR_EQ:
            strm << tabs_str << ">=" << std::endl;
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
    
    strm << tabs_str << "REL EXPR END;" << std::endl;
    return strm;
}
