#include "assstmtast.hpp"

ASS_STMT_AST*ASS_STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    IDENT_AST*ident;
    try {
        ident = IDENT_AST::read(tok, lexer);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("assign->");
        throw excp;
    }
    ASSIGN_TOKEN*ass_tok = dynamic_cast<ASSIGN_TOKEN*>(*tok);
    if (ass_tok == nullptr) {
        throw PARSER_EXCEPTION("bad assign token at");
    }
    DOMAIN_TAG*ass_op = new DOMAIN_TAG;
    *ass_op = (*tok)->get_tag();
    delete (*tok);
    (*tok) = lexer->next_token();
    try {
        LOGICAL_OR_EXPR_AST*expr = LOGICAL_OR_EXPR_AST::read(tok, lexer);
        return new ASS_STMT_AST(ident, ass_op, expr);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("assign->");
    }
}

ASS_STMT_AST::ASS_STMT_AST(IDENT_AST*ident, DOMAIN_TAG*ass_op, LOGICAL_OR_EXPR_AST*expr) : ident(ident), ass_op(ass_op), expr(expr) {}

IDENT_AST*ASS_STMT_AST::get_ident() { return ident; }
DOMAIN_TAG*ASS_STMT_AST::get_ass_op() { return ass_op; }
LOGICAL_OR_EXPR_AST*ASS_STMT_AST::get_expr() { return expr; }

ASS_STMT_AST::~ASS_STMT_AST()
{
    if (expr) {
        delete expr;
    }
}

std::ostream& operator<<(std::ostream &strm, ASS_STMT_AST &ass_stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < ass_stmt.tabs; i++) {
        tabs_str += '\t';
    }

    strm << tabs_str << "ASS_STMT BEGIN:" << std::endl;

    // ident.
    ass_stmt.ident->add_tab(ass_stmt.tabs + 1);
    strm << *(ass_stmt.ident);
    ass_stmt.ident->del_tab(ass_stmt.tabs + 1);
    switch (*(ass_stmt.get_ass_op())) {
    case DOMAIN_TAG::ASSIGN:
        strm << tabs_str << "=" << std::endl;
        break;
    case DOMAIN_TAG::ASSIGN_PLUS:
        strm << tabs_str << "+=" << std::endl;
        break;
    case DOMAIN_TAG::ASSIGN_MINUS:
        strm << tabs_str << "-=" << std::endl;
        break;
    case DOMAIN_TAG::ASSIGN_MUL:
        strm << tabs_str << "*=" << std::endl;
        break;
    case DOMAIN_TAG::ASSIGN_DIV:
        strm << tabs_str << "/=" << std::endl;
        break;
    case DOMAIN_TAG::ASSIGN_MOD:
        strm << tabs_str << "%=" << std::endl;
        break;
    default:
        // do nothing.
        break;
    }
    // expr.
    ass_stmt.expr->add_tab(ass_stmt.tabs + 1);
    strm << *(ass_stmt.expr);
    ass_stmt.expr->del_tab(ass_stmt.tabs + 1);
    
    strm << tabs_str << "ASS_STMT END;" << std::endl;
    return strm;
}
