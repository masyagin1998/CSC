#include "codegen.hpp"

#include <llvm-5.0/llvm/IR/Constant.h>
#include <llvm-5.0/llvm/ADT/APInt.h>

#include <llvm-5.0/llvm/IR/Verifier.h>

llvm::LLVMContext CODEGEN::ctx;

CODEGEN::CODEGEN(UNIT_AST*unit) : builder(ctx), module(new llvm::Module("module", ctx)), curr_func(nullptr)
{
    visit(unit);
    module->print(llvm::errs(), nullptr);
}

llvm::Value*CODEGEN::visit(UNIT_AST*unit)
{
    auto funcs = unit->get_funcs();
    for (std::size_t i = 0; i < funcs.size(); i++) {
        auto f = visit(funcs[i]);
    }
}

std::vector<llvm::Type*> CODEGEN::generate_func_args(std::vector<ARG_AST*> args_list)
{
    std::vector<llvm::Type*> llvm_args_list;

    for (std::size_t i = 0; i < args_list.size(); i++) {
        // For simplicity only primitive type are allowed.
        switch (args_list[i]->get_datatype()->get_basic_datatype()) {
        case DOMAIN_TAG::INT:
            llvm_args_list.push_back(llvm::Type::getInt32Ty(ctx));
            break;
        default:
            // TODO: throw exception.
            break;
        }
    }

    return llvm_args_list;
}

llvm::Function*CODEGEN::visit(FUNC_AST*func)
{
    // For simplicity only primitive type are allowed.
    llvm::Type*llvm_ret_datatype = nullptr;
    switch (func->get_datatype()->get_basic_datatype()) {
    case DOMAIN_TAG::INT:
        llvm_ret_datatype = llvm::Type::getInt32Ty(ctx);
        break;
    default:
        // TODO: throw exception.
        break;
    }
    
    auto llvm_name = func->get_ident()->get_ident();
    auto llvm_args_list = generate_func_args(func->get_args_list()->get_args_list());
    llvm::FunctionType*llvm_func_type = llvm::FunctionType::get(llvm_ret_datatype, llvm_args_list, false);
    llvm::Function*llvm_func = llvm::Function::Create(llvm_func_type, llvm::Function::ExternalLinkage, llvm_name, module);

    std::size_t i = 0;
    for (auto &arg: llvm_func->args()) {
        arg.setName(func->get_args_list()->get_args_list()[i]->get_ident()->get_ident()); 
    }

    curr_func = llvm_func;

    if (func->get_body() != nullptr) {
        visit(func->get_body());
    }

    llvm::verifyFunction(*llvm_func);

    return llvm_func;
}

llvm::Value*CODEGEN::visit(DATATYPE_AST*datatype)
{
    return nullptr;
}

llvm::Value*CODEGEN::visit(IDENT_AST*ident)
{
    llvm::Value*val = curr_variables[ident->get_ident()];
    if (val != nullptr) {
        
    } else {
        // TODO: throw exception.
    }
}

llvm::Value*CODEGEN::visit(ARGS_LIST_AST*args_list)
{
    return nullptr;
}

llvm::Value*CODEGEN::visit(ARG_AST*arg)
{
    return nullptr;
}

llvm::Value*CODEGEN::visit(INTEGER_AST*integer)
{
    return llvm::ConstantInt::get(ctx, llvm::APInt(32, integer->get_integer(), true));
}

llvm::Value*CODEGEN::visit(COMPOUND_STMT_AST*comp_stmt)
{
    if (curr_func == nullptr) {
        // TODO: throw exception.
    }
    
    llvm::BasicBlock*bb = llvm::BasicBlock::Create(ctx, "entry", curr_func);
    builder.SetInsertPoint(bb);

    std::map<std::string, llvm::Value*> local_variables;
    for (auto &arg : curr_func->args()) {
        curr_variables[arg.getName()] = &arg;
        local_variables[arg.getName()] = &arg;
    }

    auto decls = comp_stmt->get_decls();
    for (std::size_t i = 0; i < decls.size(); i++) {
        visit(decls[i]);
    }

    auto stmts = comp_stmt->get_stmts();
    for (std::size_t i = 0; i < stmts.size(); i++) {
        visit(stmts[i]);
    }

    for (auto it = local_variables.begin(); it != local_variables.end(); it++ ) {
        curr_variables.erase(it->first);
    }
    return nullptr;
}

llvm::Value*CODEGEN::visit(DECL_AST*decl)
{
    // TODO:
    return nullptr;
}

llvm::Value*CODEGEN::visit(STMT_AST*stmt)
{
    // ASS_STMT_AST*ass_stmt;
    COMPOUND_STMT_AST*comp_stmt;
    RET_STMT_AST*ret_stmt;

    /*if ((ass_stmt = dynamic_cast<ASS_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        return visit(ass_stmt);
        } else */
    if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        return visit(comp_stmt);
    } else if ((ret_stmt = dynamic_cast<RET_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        auto ret = visit(ret_stmt);
        builder.CreateRet(ret);
    } else {
        // TODO: throw exception.
    }

    return nullptr;
}

llvm::Value*CODEGEN::visit(ASS_STMT_AST*ass)
{
    // TODO:
    return nullptr;
}

llvm::Value*CODEGEN::visit(RET_STMT_AST*ret)
{
    return visit(ret->get_expr());
}

llvm::Value*CODEGEN::visit(LOGICAL_OR_EXPR_AST*expr)
{
    /*
    llvm::Value*first = visit(expr->get_first());
    if (expr->get_bin_op() != nullptr) {
        llvm::Value*second = visit(expr->get_second());
        switch (*(expr->get_bin_op())) {
        case DOMAIN_TAG::PLUS:
            return builder.CreateFAdd(first, second, "+");
        case DOMAIN_TAG::MINUS:
            return builder.CreateFSub(first, second, "-");
        case DOMAIN_TAG::MUL:
            return builder.CreateFMul(first, second, "*");
        case DOMAIN_TAG::DIV:
            return builder.CreateFDiv(first, second, "/");
        case DOMAIN_TAG::MOD:
            return builder.CreateFRem(first, second, "%");
        default:
            // TODO: throw exception.
            break;
        }
    }

    return first;
    */
}

llvm::Value*CODEGEN::visit(PRIMARY_EXPR_AST*primary_expr)
{
    IDENT_AST*ident;
    INTEGER_AST*integer;
    LOGICAL_OR_EXPR_AST*expr;
    
    if ((ident = dynamic_cast<IDENT_AST*>(primary_expr->get_primary_expr())) != nullptr) {
        return visit(ident);
    } else if ((integer = dynamic_cast<INTEGER_AST*>(primary_expr->get_primary_expr())) != nullptr) {
        return visit(integer);
    } else if ((expr = dynamic_cast<LOGICAL_OR_EXPR_AST*>(primary_expr->get_primary_expr())) != nullptr) {
        return visit(expr);
    } else {
        // TODO: throw exception.
    }

    return nullptr;
}

CODEGEN::~CODEGEN()
{
}
