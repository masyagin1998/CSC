#include "retstmtast.hpp"

RET_STMT_AST*RET_STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() != DOMAIN_TAG::RETURN) {
        // TODO: throw exception.
    }
    delete (*tok);
    (*tok) = lexer->next_token();
    
    LOGICAL_OR_EXPR_AST*expr = LOGICAL_OR_EXPR_AST::read(tok, lexer);
    
    return new RET_STMT_AST(expr);
}

RET_STMT_AST::RET_STMT_AST(LOGICAL_OR_EXPR_AST*expr) : expr(expr) {}

LOGICAL_OR_EXPR_AST*RET_STMT_AST::get_expr() { return expr; }

RET_STMT_AST::~RET_STMT_AST()
{
    if (expr) {
        delete expr;
    }
}

std::ostream& operator<<(std::ostream &strm, RET_STMT_AST &ret_stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < ret_stmt.tabs; i++) {
        tabs_str += '\t';
    }

    strm << tabs_str << "RET_STMT BEGIN:" << std::endl;

    // expr.
    ret_stmt.expr->add_tab(ret_stmt.tabs + 1);
    strm << *(ret_stmt.expr);
    ret_stmt.expr->del_tab(ret_stmt.tabs + 1);
    
    strm << tabs_str << "RET_STMT END;" << std::endl;
    return strm;
}
