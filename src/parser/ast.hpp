#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include "parserexception.hpp"

#include <iostream>
#include <cstdio>

class AST
{    
  public:
    void add_tab(std::size_t n);
    void del_tab(std::size_t n);

    virtual ~AST() = default;

  protected:
    std::size_t tabs;

    AST();
};

#endif  // UNIT_AST_HPP_INCLUDED
