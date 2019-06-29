#include "threeaddresscode.hpp"

SSA_VARIABLE::SSA_VARIABLE(std::string name, int version) :
    name(name),
    version(version) {}

std::ostream& operator<<(std::ostream &strm, SSA_VARIABLE&ssa_var)
{
    strm << ssa_var.name;
    if (ssa_var.version >= 0) {
        strm << "_" << ssa_var.version;
    }

    return strm;
}

SSA_INTEGER::SSA_INTEGER(int integer) : integer(integer) {}

std::ostream& operator<<(std::ostream &strm, SSA_INTEGER&ssa_int)
{
    strm << ssa_int.integer;

    return strm;
}

THREE_ADDRESS_CODE::THREE_ADDRESS_CODE(SSA_OPERAND*dest, THREE_ADDRESS_CODE_OPERATION op, SSA_OPERAND*src1, SSA_OPERAND*src2) :
    dest(dest), op(op), src1(src1), src2(src2) {}

std::ostream& operator<<(std::ostream &strm, THREE_ADDRESS_CODE&tac)
{
    if (tac.dest != nullptr) {
        SSA_VARIABLE*ssa_var;
        SSA_INTEGER*ssa_int;
        if ((ssa_var = dynamic_cast<SSA_VARIABLE*>(tac.dest)) != nullptr) {
            strm << *ssa_var;
        } else if ((ssa_int = dynamic_cast<SSA_INTEGER*>(tac.dest)) != nullptr) {
            strm << *ssa_int;
        }
    }
    switch (tac.op) {
    case THREE_ADDRESS_CODE_OPERATION::ASS:
        strm << " = ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::EQ:
        strm << " = icmp eq ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::NE:
        strm << " = icmp ne ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::LOGICAL_AND:
        strm << " = and ";
        break; 
    case THREE_ADDRESS_CODE_OPERATION::LOGICAL_OR:
        strm << " = or ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::LOGICAL_NOT:
        strm << " = not ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::MORE:
        strm << " = icmp sgt ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::LESS:
        strm << " = icmp slt ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::MORE_OR_EQ:
        strm << " = icmp sge ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::LESS_OR_EQ:
        strm << " = icmp sle ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::PLUS:
        strm << " = add ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::MINUS:
        strm << " = sub ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::MUL:
        strm << " = mul ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::SDIV:
        strm << " = sdiv ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::SMOD:
        strm << " = smod ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::ALLOCA:
        strm << "alloca ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::CONDBR:
        strm << "condbr ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::RET:
        strm << "ret ";
        break;
    case THREE_ADDRESS_CODE_OPERATION::PHI:
        strm << " = PHI ";
        break;
    }
    if (tac.src1 != nullptr) {
        SSA_VARIABLE*ssa_var;
        SSA_INTEGER*ssa_int;
        if ((ssa_var = dynamic_cast<SSA_VARIABLE*>(tac.src1)) != nullptr) {
            strm << *ssa_var;
        } else if ((ssa_int = dynamic_cast<SSA_INTEGER*>(tac.src1)) != nullptr) {
            strm << *ssa_int;
        }
    }
    if (tac.src2 != nullptr) {
        strm << ", ";
        SSA_VARIABLE*ssa_var;
        SSA_INTEGER*ssa_int;
        if ((ssa_var = dynamic_cast<SSA_VARIABLE*>(tac.src2)) != nullptr) {
            strm << *ssa_var;
        } else if ((ssa_int = dynamic_cast<SSA_INTEGER*>(tac.src2)) != nullptr) {
            strm << *ssa_int;
        }
    }

    return strm;
}

THREE_ADDRESS_CODE::~THREE_ADDRESS_CODE()
{
    if (dest != nullptr) {
        delete dest;
    }

    if (src1 != nullptr) {
        delete src1;
    }

    if (src2 != nullptr) {
        delete src2;
    }
}
