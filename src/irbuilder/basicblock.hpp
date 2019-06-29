#ifndef BASIC_BLOCK_HPP_INCLUDED
#define BASIC_BLOCK_HPP_INCLUDED

#include "lexer.hpp"

#include "funcast.hpp"

#include "threeaddresscode.hpp"

struct BASIC_BLOCK
{
    std::vector<std::size_t> preds;
    std::size_t id;
    std::vector<std::size_t> succs;

    std::vector<THREE_ADDRESS_CODE*> tacs;
    
    BASIC_BLOCK(std::vector<std::size_t> preds,
                std::size_t id,
                std::vector<std::size_t> succs,
                std::vector<THREE_ADDRESS_CODE*> tacs);

    friend std::ostream& operator<<(std::ostream &strm, BASIC_BLOCK&bb);
    
    ~BASIC_BLOCK();
};

#endif  // BASIC_BLOCK_HPP_INCLUDED
