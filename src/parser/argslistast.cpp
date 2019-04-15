#include "argslistast.hpp"

ARGS_LIST_AST*ARGS_LIST_AST::read(TOKEN**tok, LEXER*lexer)
{   
    if ((*tok)->get_tag() != DOMAIN_TAG::LPAREN) {
        throw PARSER_EXCEPTION("bad beginning of args list at");
    }
    delete *tok;
    (*tok) = lexer->next_token();

    std::vector<ARG_AST*> args_list;

    while ((*tok)->get_tag() != DOMAIN_TAG::RPAREN) {
        try {
            ARG_AST*arg = ARG_AST::read(tok, lexer);
            args_list.push_back(arg);
        } catch (PARSER_EXCEPTION &excp) {
            excp.prepend_exception("args_list->");
            throw excp;
        }
        if (((*tok)->get_tag() != DOMAIN_TAG::RPAREN) &&
            ((*tok)->get_tag() != DOMAIN_TAG::COMMA)) {
            throw PARSER_EXCEPTION("bad ending of args list at");
        }
        if ((*tok)->get_tag() == DOMAIN_TAG::COMMA) {
            delete *tok;
            (*tok) = lexer->next_token();
        }
    }
    delete *tok;
    (*tok) = lexer->next_token();

    return new ARGS_LIST_AST(args_list);
}

ARGS_LIST_AST::ARGS_LIST_AST(std::vector<ARG_AST*> args_list) : args_list(args_list) {}

std::vector<ARG_AST*> ARGS_LIST_AST::get_args_list() { return args_list; }

void ARGS_LIST_AST::set_args_list(std::vector<ARG_AST*> args_list) { this->args_list = args_list; }

ARGS_LIST_AST::~ARGS_LIST_AST()
{
    for (std::size_t i = 0; i < args_list.size(); i++) {
        delete args_list[i];
    }
}

std::ostream& operator<<(std::ostream &strm, ARGS_LIST_AST &args_list)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < args_list.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "ARGS_LIST BEGIN:" << std::endl;
    for (std::size_t i = 0; i < args_list.args_list.size(); i++) {
        args_list.args_list[i]->add_tab(args_list.tabs + 1);
        strm << *args_list.args_list[i];
        args_list.args_list[i]->del_tab(args_list.tabs + 1);
    }
    strm << tabs_str << "ARGS_LIST END;" << std::endl;
    return strm;
}
