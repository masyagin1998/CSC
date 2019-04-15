#ifndef BIN_OP_TOKENS_HPP_INCLUDED
#define BIN_OP_TOKENS_HPP_INCLUDED

#include "token.hpp"
#include "position.hpp"
#include "domaintag.hpp"

#include <string>
#include <ostream>

class BIN_OP_TOKEN : public TOKEN
{
  private:
    static const std::string BIN_OP_PLUS;
    static const std::string BIN_OP_MINUS;
    static const std::string BIN_OP_MUL;
    static const std::string BIN_OP_DIV;
    static const std::string BIN_OP_MOD;
    
    std::string bin_op;
    
  public:
    static DOMAIN_TAG is(POSITION pos);
    
    static BIN_OP_TOKEN*read(POSITION pos, DOMAIN_TAG tag);
    
    BIN_OP_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string brace);

    std::string get_bin_op() const;

    friend std::ostream& operator<<(std::ostream &strm, const BIN_OP_TOKEN &tok);
};

#endif  // BIN_OP_TOKENS_HPP_INCLUDED
