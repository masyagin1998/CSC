 #include "stmtast.hpp"

#include "assstmtast.hpp"
#include "compoundstmtast.hpp"
#include "retstmtast.hpp"

STMT_AST*STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() == DOMAIN_TAG::IDENT) {
        return new STMT_AST(ASS_STMT_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::LBRACE) {
        return new STMT_AST(COMPOUND_STMT_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::RETURN) {
        STMT_AST*stmt = new STMT_AST(RET_STMT_AST::read(tok, lexer));
        if ((*tok)->get_tag() != DOMAIN_TAG::SEMICOLON) {
            // TODO: throw exception;
        }
        delete (*tok);
        (*tok) = lexer->next_token();
        return stmt;
    } else {
        // TODO: throw exception.
    }

    return nullptr;
}

STMT_AST::STMT_AST(AST*stmt) :
    stmt(stmt) {}

AST*STMT_AST::get_stmt() { return stmt; };

STMT_AST::~STMT_AST()
{
    delete stmt;
}

std::ostream& operator<<(std::ostream &strm, STMT_AST &stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < stmt.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "STMT BEGIN:" << std::endl;
    ASS_STMT_AST*ass_stmt;
    COMPOUND_STMT_AST*comp_stmt;
    RET_STMT_AST*ret_stmt;

    stmt.stmt->add_tab(stmt.tabs + 1);
    /*
    if ((ass_stmt = dynamic_cast<ASS_STMT_AST*>(stmt.stmt)) != nullptr) {
        // strm << *ass_stmt;
        } else */if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *comp_stmt;
    } else if ((ret_stmt = dynamic_cast<RET_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *ret_stmt;
    }
    stmt.stmt->del_tab(stmt.tabs + 1);
    
    strm << tabs_str << "STMT END;" << std::endl;
    return strm;
}
