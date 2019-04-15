#ifndef CODEGEN_HPP_INCLUDED
#define CODEGEN_HPP_INCLUDED

#include "lexer.hpp"

#include "unitast.hpp"
#include "integerast.hpp"
#include "retstmtast.hpp"
#include "logicalexprast.hpp"
#include "primaryexprast.hpp"
#include "assstmtast.hpp"

#include <llvm-5.0/llvm/IR/LLVMContext.h>
#include <llvm-5.0/llvm/IR/IRBuilder.h>

class CODEGEN
{
  private:
    static llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder;
    
    llvm::Module*module;
    std::map<std::string, llvm::Value*> curr_variables;
    llvm::Function*curr_func;

    llvm::Value*visit(UNIT_AST*unit);
    std::vector<llvm::Type*> generate_func_args(std::vector<ARG_AST*> args_list);
    llvm::Function*visit(FUNC_AST*func);
    llvm::Value*visit(DATATYPE_AST*datatype);
    llvm::Value*visit(IDENT_AST*ident);
    llvm::Value*visit(ARGS_LIST_AST*args_list);
    llvm::Value*visit(ARG_AST*arg);
    llvm::Value*visit(INTEGER_AST*integer);
    llvm::Value*visit(COMPOUND_STMT_AST*stmt);
    llvm::Value*visit(DECL_AST*decl);
    llvm::Value*visit(STMT_AST*stmt);
    llvm::Value*visit(ASS_STMT_AST*ass);
    llvm::Value*visit(RET_STMT_AST*ret);
    llvm::Value*visit(LOGICAL_OR_EXPR_AST*expr);
    llvm::Value*visit(PRIMARY_EXPR_AST*expr);
    
  public:
    CODEGEN(UNIT_AST*unit);

    ~CODEGEN();
};

#endif  // CODEGEN_HPP_INCLUDED
