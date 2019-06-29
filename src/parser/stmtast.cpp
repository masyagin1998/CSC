 #include "stmtast.hpp"

#include "assstmtast.hpp"
#include "compoundstmtast.hpp"
#include "retstmtast.hpp"
#include "ifstmtast.hpp"
#include "loopstmtast.hpp"

STMT_AST*STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() == DOMAIN_TAG::IDENT) {
        STMT_AST*stmt = new STMT_AST(ASS_STMT_AST::read(tok, lexer));
        if ((*tok)->get_tag() != DOMAIN_TAG::SEMICOLON) {
            throw PARSER_EXCEPTION("bad ending of ass_stmt at");
        }
        delete (*tok);
        (*tok) = lexer->next_token();
        return stmt;
    } else if ((*tok)->get_tag() == DOMAIN_TAG::LBRACE) {
        return new STMT_AST(COMPOUND_STMT_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::RETURN) {
        STMT_AST*stmt = new STMT_AST(RET_STMT_AST::read(tok, lexer));
        if ((*tok)->get_tag() != DOMAIN_TAG::SEMICOLON) {
            throw PARSER_EXCEPTION("bad ending of ret_stmt at");
        }
        delete (*tok);
        (*tok) = lexer->next_token();
        return stmt;
    } else if ((*tok)->get_tag() == DOMAIN_TAG::FOR) {
        // return new FOR_STMT_AST(FOR_STMT_AST::read(tok, lexer)); // TODO.
    } else if ((*tok)->get_tag() == DOMAIN_TAG::WHILE) {
        return new STMT_AST(WHILE_STMT_AST::read(tok, lexer));
    } else if ((*tok)->get_tag() == DOMAIN_TAG::DO) {
        // return new DO_WHILE_STMT_AST(DO_WHILE_STMT_AST::read(tok, lexer)); // TODO.
    }  else if ((*tok)->get_tag() == DOMAIN_TAG::IF) {
        return new STMT_AST(IF_STMT_AST::read(tok, lexer));
    }

    throw PARSER_EXCEPTION("unknown stmt at");
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
    WHILE_STMT_AST*while_stmt;
    IF_STMT_AST*if_stmt;

    stmt.stmt->add_tab(stmt.tabs + 1);
    if ((ass_stmt = dynamic_cast<ASS_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *ass_stmt;
    } else if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *comp_stmt;
    } else if ((ret_stmt = dynamic_cast<RET_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *ret_stmt;
    } else if ((while_stmt = dynamic_cast<WHILE_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *while_stmt;
    } else if ((if_stmt = dynamic_cast<IF_STMT_AST*>(stmt.stmt)) != nullptr) {
        strm << *if_stmt;
    }
    stmt.stmt->del_tab(stmt.tabs + 1);
    
    strm << tabs_str << "STMT END;" << std::endl;
    return strm;
}
