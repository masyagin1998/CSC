#ifndef IR_BUILDER_HPP_INCLUDED
#define IR_BUILDER_HPP_INCLUDED

#include "lexer.hpp"

#include "funcast.hpp"
#include "loopstmtast.hpp"
#include "ifstmtast.hpp"

#include "stmtast.hpp"
#include "declast.hpp"
#include "assstmtast.hpp"
#include "retstmtast.hpp"

#include "logicalexprast.hpp"
#include "eqexprast.hpp"
#include "relexprast.hpp"
#include "binexprast.hpp"
#include "unaryexprast.hpp"
#include "primaryexprast.hpp"
#include "integerast.hpp"
#include "identast.hpp"

#include "controlflowgraph.hpp"

#include <tuple>

class IR_BUILDER
{
  private:
    CFG*cfg;

    // tmp variables.
    static const std::string logandtmp_str;
    int                      logandtmp_version;
    static const std::string logortmp_str;
    int                      logortmp_version;
    static const std::string lognottmp_str;
    int                      lognottmp_version;

    static const std::string eqtmp_str;
    int                      eqtmp_version;
    static const std::string netmp_str;
    int                      netmp_version;

    static const std::string lesstmp_str;
    int                      lesstmp_version;
    static const std::string moretmp_str;
    int                      moretmp_version;
    static const std::string lesseqtmp_str;
    int                      lesseqtmp_version;
    static const std::string moreeqtmp_str;
    int                      moreeqtmp_version;
    
    static const std::string addtmp_str;
    int                      addtmp_version;
    static const std::string subtmp_str;
    int                      subtmp_version;

    static const std::string multmp_str;
    int                      multmp_version;
    static const std::string sdivtmp_str;
    int                      sdivtmp_version;
    static const std::string smodtmp_str;
    int                      smodtmp_version;
    
    void visit_block(FUNC_AST*func, std::vector<BASIC_BLOCK*>&bbs);
    void visit_block(IF_STMT_AST*if_stmt, std::vector<BASIC_BLOCK*>&bbs);
    void visit_block(WHILE_STMT_AST*while_stmt, std::vector<BASIC_BLOCK*>&bbs);

    int visit_stmt(STMT_AST*stmt, std::vector<THREE_ADDRESS_CODE*>&tacs);
    void visit_inblock(DECL_AST*decl, std::vector<THREE_ADDRESS_CODE*>&tacs);
    void visit_inblock(ASS_STMT_AST*ass_stmt, std::vector<THREE_ADDRESS_CODE*>&tacs);
    void visit_inblock(RET_STMT_AST*ret_stmt, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(LOGICAL_OR_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(LOGICAL_AND_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(EQ_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(REL_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(ADD_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(MULT_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(LEFT_UNARY_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(PRIMARY_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(IDENT_AST*ident, std::vector<THREE_ADDRESS_CODE*>&tacs);
    SSA_OPERAND*visit_expr(INTEGER_AST*integer, std::vector<THREE_ADDRESS_CODE*>&tacs);
    
  public:
    IR_BUILDER(FUNC_AST*func);
    
    CFG*get_cfg();
    
    ~IR_BUILDER() = default;
};

#endif  // IR_BUILDER_HPP_INCLUDED
