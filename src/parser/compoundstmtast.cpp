#include "compoundstmtast.hpp"

COMPOUND_STMT_AST*COMPOUND_STMT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() != DOMAIN_TAG::LBRACE) {
        // TODO: throw exception.
    }
    delete (*tok);
    (*tok) = lexer->next_token();

    try {
        std::vector<DECL_AST*> decls;
        // For simplicity only INT type is allowed.
        while ((*tok)->get_tag() == DOMAIN_TAG::INT) {
            DECL_AST*decl = DECL_AST::read(tok, lexer);
            decls.push_back(decl);
        }

        std::vector<STMT_AST*> stmts;

        // TODO.
        while ((*tok)->get_tag() != DOMAIN_TAG::RBRACE) {
            STMT_AST*stmt = STMT_AST::read(tok, lexer);
            stmts.push_back(stmt);
        }

        delete (*tok);
        (*tok) = lexer->next_token();

        return new COMPOUND_STMT_AST(decls, stmts);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("comp_stmt->");
        throw excp;
    }
}

COMPOUND_STMT_AST::COMPOUND_STMT_AST(std::vector<DECL_AST*> decls, std::vector<STMT_AST*> stmts) : decls(decls), stmts(stmts) {}

std::vector<STMT_AST*> COMPOUND_STMT_AST::get_stmts() { return stmts; }
void COMPOUND_STMT_AST::set_stmts(std::vector<STMT_AST*> stmts) { this->stmts = stmts; }

std::vector<DECL_AST*> COMPOUND_STMT_AST::get_decls() { return decls; }
void COMPOUND_STMT_AST::set_decls(std::vector<DECL_AST*> decls) { this->decls = decls; }

COMPOUND_STMT_AST::~COMPOUND_STMT_AST()
{
    // decls.
    for (std::size_t i = 0; i < decls.size(); i++) {
        delete decls[i];
    }

    for (std::size_t i = 0; i < stmts.size(); i++) {
        delete stmts[i];
    }
}

std::ostream& operator<<(std::ostream &strm, COMPOUND_STMT_AST &stmt)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < stmt.tabs; i++) {
        tabs_str += '\t';
    }

    strm << tabs_str << "COMPOUND_STMT BEGIN:" << std::endl;
    // decls.
    for (std::size_t i = 0; i < stmt.decls.size(); i++) {
        stmt.decls[i]->add_tab(stmt.tabs + 1);
        strm << *stmt.decls[i];
        stmt.decls[i]->del_tab(stmt.tabs + 1);
    }
    // stmts.
    for (std::size_t i = 0; i < stmt.stmts.size(); i++) {
        stmt.stmts[i]->add_tab(stmt.tabs + 1);
        strm << *stmt.stmts[i];
        stmt.stmts[i]->del_tab(stmt.tabs + 1);
    }
    strm << tabs_str << "COMPOUND_STMT END;" << std::endl;
    return strm;
}
