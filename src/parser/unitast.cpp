#include "unitast.hpp"

UNIT_AST*UNIT_AST::read(TOKEN**tok, LEXER*lexer)
{
    if ((*tok)->get_tag() == DOMAIN_TAG::END_OF_FILE) {
        return nullptr;
    }

    try {
        std::vector<FUNC_AST*>funcs;
        while ((*tok)->get_tag() != DOMAIN_TAG::END_OF_FILE) {
            FUNC_AST*func = FUNC_AST::read(tok, lexer);
            funcs.push_back(func);
        }
        delete (*tok);
        return new UNIT_AST(funcs);
    } catch (PARSER_EXCEPTION &excp) {
        std::cerr << excp;
        return nullptr;
    }
}

UNIT_AST::UNIT_AST(std::vector<FUNC_AST*> funcs) : funcs(funcs) {};

std::vector<FUNC_AST*> UNIT_AST::get_funcs() { return funcs; }
void UNIT_AST::set_funcs(std::vector<FUNC_AST*> funcs) { this->funcs = funcs; }

UNIT_AST::~UNIT_AST()
{
    for (std::size_t i = 0; i < funcs.size(); i++) {
        delete funcs[i];
    }
}

std::ostream& operator<<(std::ostream &strm, UNIT_AST &unit)
{
    strm << "UNIT BEGIN:" << std::endl;
    for (std::size_t i = 0; i < unit.funcs.size(); i++) {
        unit.funcs[i]->add_tab(1);
        strm << *unit.funcs[i];
        unit.funcs[i]->del_tab(1);
        strm << std::endl;
    }
    strm << "UNIT END;" << std::endl;
    return strm;
}
