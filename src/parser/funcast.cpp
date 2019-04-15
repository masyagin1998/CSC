#include "funcast.hpp"

FUNC_AST*FUNC_AST::read(TOKEN**tok, LEXER*lexer)
{
    try {
        DATATYPE_AST*datatype = DATATYPE_AST::read(tok, lexer);
        IDENT_AST*ident = IDENT_AST::read(tok, lexer);
        ARGS_LIST_AST*args_list = ARGS_LIST_AST::read(tok, lexer);
        COMPOUND_STMT_AST*body = COMPOUND_STMT_AST::read(tok, lexer);
        return new FUNC_AST(datatype, ident, args_list, body);
    } catch (PARSER_EXCEPTION &excp) {
        excp.prepend_exception("func->");
        throw excp;
    }
}

FUNC_AST::FUNC_AST(DATATYPE_AST*datatype,
                   IDENT_AST*ident,
                   ARGS_LIST_AST*args_list,
                   COMPOUND_STMT_AST*body) :
    datatype(datatype),
    ident(ident),
    args_list(args_list),
    body(body) {}

DATATYPE_AST*FUNC_AST::get_datatype() { return datatype; }
IDENT_AST*FUNC_AST::get_ident() { return ident; };
ARGS_LIST_AST*FUNC_AST::get_args_list() { return args_list; };
COMPOUND_STMT_AST*FUNC_AST::get_body() { return body; };

FUNC_AST::~FUNC_AST()
{
    delete datatype;
    delete ident;
    delete args_list;
    delete body;
}

std::ostream& operator<<(std::ostream &strm, FUNC_AST &func)
{
    std::string tabs_str = "";
    for (std::size_t i = 0; i < func.tabs; i++) {
        tabs_str += '\t';
    }
    
    strm << tabs_str << "FUNC BEGIN:" << std::endl;
    // datatype.
    func.datatype->add_tab(func.tabs + 1);
    strm << *(func.datatype);
    func.datatype->del_tab(func.tabs + 1);
    // ident.
    func.ident->add_tab(func.tabs + 1);
    strm << *(func.ident);
    func.ident->del_tab(func.tabs + 1);
    // args_list.
    func.args_list->add_tab(func.tabs + 1);
    strm << *(func.args_list);
    func.args_list->del_tab(func.tabs + 1);
    // body.
    func.body->add_tab(func.tabs + 1);
    strm << *(func.body);
    func.body->del_tab(func.tabs + 1);
    
    strm << tabs_str << "FUNC END;" << std::endl;
    return strm;
}
