#include "ast.hpp"

AST::AST() : tabs(0) {}

void AST::add_tab(std::size_t n)
{
    tabs += n;
}

void AST::del_tab(std::size_t n)
{
    tabs -= n;
}
