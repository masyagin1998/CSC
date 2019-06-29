#ifndef CODEGEN_HPP_INCLUDED
#define CODEGEN_HPP_INCLUDED

#include "lexer.hpp"

#include "unitast.hpp"
#include "integerast.hpp"
#include "retstmtast.hpp"
#include "logicalexprast.hpp"
#include "eqexprast.hpp"
#include "relexprast.hpp"
#include "primaryexprast.hpp"
#include "assstmtast.hpp"
#include "loopstmtast.hpp"
#include "ifstmtast.hpp"
#include "relexprast.hpp"
#include "unaryexprast.hpp"

#include <llvm-5.0/llvm/IR/LLVMContext.h>
#include <llvm-5.0/llvm/IR/IRBuilder.h>
#include <llvm-5.0/llvm/IR/Constant.h>
#include <llvm-5.0/llvm/ADT/APInt.h>
#include <llvm-5.0/llvm/IR/Verifier.h>
#include <llvm-5.0/llvm/IR/LegacyPassManager.h>

#include <llvm-5.0/llvm/Support/FileSystem.h>
#include <llvm-5.0/llvm/Support/Host.h>
#include <llvm-5.0/llvm/Support/raw_ostream.h>
#include <llvm-5.0/llvm/Support/TargetRegistry.h>
#include <llvm-5.0/llvm/Support/TargetSelect.h>
#include <llvm-5.0/llvm/Target/TargetMachine.h>
#include <llvm-5.0/llvm/Target/TargetOptions.h>

class CODEGEN
{
  private:
    static llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder;
    
    llvm::Module*module;
    std::map<std::string, llvm::AllocaInst*> curr_variables;
    llvm::Function*curr_func;
    llvm::BasicBlock*curr_bb;

    void visit(UNIT_AST*unit);
    std::vector<llvm::Type*> generate_func_args(std::vector<ARG_AST*> args_list);
    llvm::AllocaInst*create_entry_block_alloca(llvm::Function *llvm_func, ARG_AST*arg);
    llvm::Function*visit(FUNC_AST*func);
    llvm::Value*visit(IDENT_AST*ident);
    llvm::Value*visit(INTEGER_AST*integer);
    void visit(COMPOUND_STMT_AST*stmt);
    llvm::Value*visit(DECL_AST*decl);
    void visit(STMT_AST*stmt);
    void visit(ASS_STMT_AST*ass_stmt);
    llvm::Value*visit(RET_STMT_AST*ret_stmt);
    void visit(WHILE_STMT_AST*while_stmt);
    void visit(IF_STMT_AST*if_stmt);
    llvm::BasicBlock*visit(IF_BRANCH_STMT_AST*if_br_stmt);
    llvm::Value*visit(LOGICAL_OR_EXPR_AST*expr);
    llvm::Value*visit(LOGICAL_AND_EXPR_AST*expr);
    llvm::Value*visit(EQ_EXPR_AST*expr);
    llvm::Value*visit(REL_EXPR_AST*expr);
    llvm::Value*visit(ADD_EXPR_AST*expr);
    llvm::Value*visit(MULT_EXPR_AST*expr);
    llvm::Value*visit(LEFT_UNARY_EXPR_AST*expr);
    llvm::Value*visit(PRIMARY_EXPR_AST*expr);
    
  public:
    CODEGEN(UNIT_AST*unit);
    llvm::Module*get_module();
    void generate_obj(std::string objname);
    ~CODEGEN();
};

#endif  // CODEGEN_HPP_INCLUDED
