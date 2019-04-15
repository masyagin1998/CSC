#include "primaryexprast.hpp"

#include "identast.hpp"
#include "integerast.hpp"
#include "logicalexprast.hpp"

PRIMARY_EXPR_AST*PRIMARY_EXPR_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() == DOMAIN_TAG::IDENT) {
        std::string ident = (dynamic_cast<IDENT_TOKEN*>((*tok)))->get_ident();
        delete (*tok);
        (*tok) = lexer->next_token();
        return new PRIMARY_EXPR_AST(new IDENT_AST(ident));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::INTEGER) {
        int integer = (dynamic_cast<INTEGER_TOKEN*>((*tok)))->get_integer();
        delete (*tok);
        (*tok) = lexer->next_token();
        return new PRIMARY_EXPR_AST(new INTEGER_AST(integer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::LPAREN) {
        delete (*tok);
        (*tok) = lexer->next_token();
        LOGICAL_OR_EXPR_AST*expr = LOGICAL_OR_EXPR_AST::read(tok, lexer);
        if ((*tok)->get_tag() != DOMAIN_TAG::RPAREN) {
            // TODO: throw exception.
        }
        delete (*tok);
        (*tok) = lexer->next_token();
        return new PRIMARY_EXPR_AST(expr);
    } else {
        // TODO: throw exception.
    }

    return nullptr;
}

PRIMARY_EXPR_AST::PRIMARY_EXPR_AST(AST*primary_expr) :
    primary_expr(primary_expr) {}

AST*PRIMARY_EXPR_AST::get_primary_expr() { return primary_expr; };

PRIMARY_EXPR_AST::~PRIMARY_EXPR_AST()
{
    delete primary_expr;
}

std::ostream& operator<<(std::ostream &strm, PRIMARY_EXPR_AST &primary_expr)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < primary_expr.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "PRIMARY_EXPR BEGIN:" << std::endl;
    IDENT_AST*ident;
    INTEGER_AST*integer;
    LOGICAL_OR_EXPR_AST*expr;

    primary_expr.primary_expr->add_tab(primary_expr.tabs + 1);
    if ((ident = dynamic_cast<IDENT_AST*>(primary_expr.primary_expr)) != nullptr) {
        strm << *ident;
    } else if ((integer = dynamic_cast<INTEGER_AST*>(primary_expr.primary_expr)) != nullptr) {
        strm << *integer;
    } else if ((expr = dynamic_cast<LOGICAL_OR_EXPR_AST*>(primary_expr.primary_expr)) != nullptr) {
        strm << *expr;
    }
    primary_expr.primary_expr->del_tab(primary_expr.tabs + 1);
    strm << tabs_str << "PRIMARY_EXPR END;" << std::endl;
    return strm;
}
