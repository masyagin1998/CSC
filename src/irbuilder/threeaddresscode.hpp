#ifndef THREE_ADDRESS_CODE_HPP_INCLUDED
#define THREE_ADDRESS_CODE_HPP_INCLUDED

#include "lexer.hpp"

#include "domaintag.hpp"
#include "funcast.hpp"

#include <iostream>

enum class THREE_ADDRESS_CODE_OPERATION
{
    /* High-level operations. */
    // =
    ASS,
 
    // == и !=
    EQ,
    NE,

    // &&, ||, !
    LOGICAL_OR,
    LOGICAL_AND,
    LOGICAL_NOT,

    // >, <, >=, <=
    MORE,
    LESS,
    MORE_OR_EQ,
    LESS_OR_EQ,

    // +, -
    PLUS,
    MINUS,

    // *, /, %
    MUL,
    SDIV,
    SMOD,

    // create new variable
    ALLOCA,

    // conditional branch
    CONDBR,

    // return from function
    RET,

    /* Low-level operations. */
    // phi-function.
    PHI,
};

struct SSA_OPERAND
{
  public:
    virtual ~SSA_OPERAND() = default;

  protected:
    SSA_OPERAND() = default;
};

struct SSA_VARIABLE : public SSA_OPERAND
{
    std::string name;
    int         version;

    SSA_VARIABLE(std::string name, int version);
    friend std::ostream& operator<<(std::ostream&strm, SSA_VARIABLE&ssa_var);
};

struct SSA_INTEGER : public SSA_OPERAND
{
    int integer;

    SSA_INTEGER(int integer);
    friend std::ostream& operator<<(std::ostream&strm, SSA_INTEGER&ssa_int);
};

struct THREE_ADDRESS_CODE
{
    SSA_OPERAND*dest;
    THREE_ADDRESS_CODE_OPERATION op;
    SSA_OPERAND*src1;
    SSA_OPERAND*src2;
    
    THREE_ADDRESS_CODE(SSA_OPERAND*dest, THREE_ADDRESS_CODE_OPERATION op, SSA_OPERAND*src1, SSA_OPERAND*src2);
    
    friend std::ostream& operator<<(std::ostream&strm, THREE_ADDRESS_CODE&tac);
    
    ~THREE_ADDRESS_CODE();
};

#endif  // THREE_ADDRESS_CODE_HPP_INCLUDED
