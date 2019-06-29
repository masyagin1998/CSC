#include "basicblock.hpp"

BASIC_BLOCK::BASIC_BLOCK(
    std::vector<std::size_t> preds,
    std::size_t id,
    std::vector<std::size_t> succs,
    std::vector<THREE_ADDRESS_CODE*> tacs) :
    preds(preds), id(id), succs(succs), tacs(tacs) {}

std::ostream& operator<<(std::ostream &strm, BASIC_BLOCK&bb)
{
    strm << "\t";
    strm << "bb: ";
    strm << "(";
    if (bb.preds.size() != 0) {
        for (std::size_t i = 0; i < bb.preds.size() - 1; i++) {
            strm << bb.preds[i] << ", ";
        }
        strm << bb.preds[bb.preds.size() - 1];
    }
    strm << ")";

    strm << " -> (" << bb.id << ") -> ";

    strm << "(";
    if (bb.succs.size() != 0) {
        for (std::size_t i = 0; i < bb.succs.size() - 1; i++) {
            strm << bb.succs[i] << ", ";
        }
        strm << bb.succs[bb.succs.size() - 1];
    }
    strm << ")";

    strm << std::endl;
    for (std::size_t i = 0; i < bb.tacs.size(); i++) {
        strm << "\t\t" << *bb.tacs[i];
        if (bb.tacs[i]->op == THREE_ADDRESS_CODE_OPERATION::CONDBR) {
            strm << " ";
            strm << "label %" << bb.succs[0];
            strm << ", ";
            strm << "label %" << bb.succs[1];
        }
        strm << std::endl;
    }

    if (bb.succs.size() == 1) {
        strm << "\t\t" << "br " << "label %" << bb.succs[0] << std::endl;
    }

    return strm;
}

BASIC_BLOCK::~BASIC_BLOCK()
{
    for (std::size_t i = 0; i < tacs.size(); i++) {
        delete tacs[i];
    }
}
