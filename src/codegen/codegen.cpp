#include "codegen.hpp"

llvm::LLVMContext CODEGEN::ctx;

CODEGEN::CODEGEN(UNIT_AST*unit) : builder(ctx), module(new llvm::Module("module", ctx)), curr_func(nullptr)
{
    if (unit != nullptr) {
        visit(unit);
    }
}

void CODEGEN::visit(UNIT_AST*unit)
{
    auto funcs = unit->get_funcs();
    for (std::size_t i = 0; i < funcs.size(); i++) {
        visit(funcs[i]);
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

llvm::AllocaInst*CODEGEN::create_entry_block_alloca(llvm::Function *llvm_func, ARG_AST*arg) {
    llvm::IRBuilder<>TmpB(&llvm_func->getEntryBlock(), llvm_func->getEntryBlock().begin());
    const char*name = arg->get_ident()->get_ident().c_str();
    switch (arg->get_datatype()->get_basic_datatype()) {
    case DOMAIN_TAG::INT:
        return TmpB.CreateAlloca(llvm::Type::getInt32Ty(ctx), nullptr, name);
    default:
        // TODO: throw exception.
        break;
    }
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

    curr_func = llvm_func;

    if (func->get_body() != nullptr) {
        llvm::BasicBlock*bb = llvm::BasicBlock::Create(ctx, "entry", curr_func);
        builder.SetInsertPoint(bb);
        std::size_t i = 0;
        for (auto &arg: llvm_func->args()) {
            std::string name = func->get_args_list()->get_args_list()[i]->get_ident()->get_ident();
            arg.setName(name);
            llvm::AllocaInst*alloca = create_entry_block_alloca(llvm_func, func->get_args_list()->get_args_list()[i]);
            builder.CreateStore(&arg, alloca);
            curr_variables[name] = alloca;
            i++;
        }
        visit(func->get_body());
    }
    // To avoid llvm failure when there is no primary return statement at the end of function.
    llvm::BasicBlock*last = &(curr_func->getBasicBlockList().back());
    builder.SetInsertPoint(last);
    builder.CreateRet(llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(ctx), 0, true));

    llvm::verifyFunction(*llvm_func);

    return llvm_func;
}

llvm::Value*CODEGEN::visit(IDENT_AST*ident)
{
    llvm::Value*llvm_ident = curr_variables[ident->get_ident()];
    if (llvm_ident != nullptr) {
        return builder.CreateLoad(llvm_ident, ident->get_ident().c_str());
    } else {
        // TODO: throw exception.
    }
}

llvm::Value*CODEGEN::visit(INTEGER_AST*integer)
{
    return llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(ctx), integer->get_integer(), true);
}

void CODEGEN::visit(COMPOUND_STMT_AST*comp_stmt)
{
    if (curr_func == nullptr) {
        // TODO: throw exception.
    }

    auto decls = comp_stmt->get_decls();
    for (std::size_t i = 0; i < decls.size(); i++) {
        visit(decls[i]);
    }

    auto stmts = comp_stmt->get_stmts();
    for (std::size_t i = 0; i < stmts.size(); i++) {
        visit(stmts[i]);
    }
}

llvm::Value*CODEGEN::visit(DECL_AST*decl)
{
    llvm::Value*initializer = llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true));
    if (decl->get_expr() != nullptr) {
        initializer = visit(decl->get_expr());
    }
    ARG_AST*arg = new ARG_AST(decl->get_datatype(), decl->get_ident());
    llvm::AllocaInst *alloca = create_entry_block_alloca(curr_func, arg);
    builder.CreateStore(initializer, alloca);
    curr_variables[decl->get_ident()->get_ident()] = alloca;
    return nullptr;
}

void CODEGEN::visit(STMT_AST*stmt)
{
    ASS_STMT_AST*ass_stmt;
    COMPOUND_STMT_AST*comp_stmt;
    RET_STMT_AST*ret_stmt;
    WHILE_STMT_AST*while_stmt;
    IF_STMT_AST*if_stmt;

    if ((ass_stmt = dynamic_cast<ASS_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        visit(ass_stmt);
    } else if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        visit(comp_stmt);
    } else if ((ret_stmt = dynamic_cast<RET_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        auto ret = visit(ret_stmt);
        builder.CreateRet(ret);
    } else if ((while_stmt = dynamic_cast<WHILE_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        visit(while_stmt);
    } else if ((if_stmt = dynamic_cast<IF_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        visit(if_stmt);
    } else {
        // TODO: throw exception.
    }
}

void CODEGEN::visit(ASS_STMT_AST*ass)
{
    auto v = curr_variables[ass->get_ident()->get_ident()];
    switch (*(ass->get_ass_op())) {
    case DOMAIN_TAG::ASSIGN:
        builder.CreateStore(visit(ass->get_expr()), v);
        break;
    case DOMAIN_TAG::ASSIGN_PLUS:
        builder.CreateStore(builder.CreateAdd(v, visit(ass->get_expr()), "addtmp"), v);
        break;
    case DOMAIN_TAG::ASSIGN_MINUS:
        builder.CreateStore(builder.CreateSub(v, visit(ass->get_expr()), "addsub"), v);
        break;
    case DOMAIN_TAG::ASSIGN_DIV:
        builder.CreateStore(builder.CreateSDiv(v, visit(ass->get_expr()), "divtmp"), v);
        break;
    case DOMAIN_TAG::ASSIGN_MOD:
        builder.CreateStore(builder.CreateSRem(v, visit(ass->get_expr()), "modtmp"), v);
        break;
    default:
        // TODO: throw exception.
        break;
    }
}

llvm::Value*CODEGEN::visit(RET_STMT_AST*ret)
{
    return visit(ret->get_expr());
}

void CODEGEN::visit(WHILE_STMT_AST*while_stmt)
{
    llvm::BasicBlock *while_loop = llvm::BasicBlock::Create(ctx, "while_loop");
    builder.CreateBr(while_loop);
    curr_func->getBasicBlockList().push_back(while_loop);
    llvm::BasicBlock *while_begin = llvm::BasicBlock::Create(ctx, "while_begin", curr_func);
    builder.SetInsertPoint(while_begin);
    visit(while_stmt->get_comp_stmt());
    builder.CreateBr(while_loop);
    llvm::BasicBlock *while_end = llvm::BasicBlock::Create(ctx, "while_end", curr_func);
    builder.SetInsertPoint(while_loop);
    llvm::Value*cond = visit(while_stmt->get_logical());
    builder.CreateCondBr(cond, while_begin, while_end);
    builder.SetInsertPoint(while_end);
}

void CODEGEN::visit(IF_STMT_AST*if_stmt)
{
    llvm::BasicBlock*merge = visit(if_stmt->get_first());
    if (if_stmt->get_second() != nullptr) {
        COMPOUND_STMT_AST*comp_stmt;
        IF_STMT_AST*if_br_stmt;
        if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(if_stmt->get_second())) != nullptr) {
            visit(comp_stmt);
        } else if ((if_br_stmt = dynamic_cast<IF_STMT_AST*>(if_stmt->get_second())) != nullptr) {
            visit(if_br_stmt);
        }
    }
    
    builder.CreateBr(merge);    
    curr_func->getBasicBlockList().push_back(merge);
    builder.SetInsertPoint(merge);
}

llvm::BasicBlock*CODEGEN::visit(IF_BRANCH_STMT_AST*if_br_stmt)
{
    llvm::Value*cond = visit(if_br_stmt->get_logical());

    llvm::BasicBlock *cond_true = llvm::BasicBlock::Create(ctx, "cond_true", curr_func);
    llvm::BasicBlock *cond_false = llvm::BasicBlock::Create(ctx, "cond_false", curr_func);
    llvm::BasicBlock *merge = llvm::BasicBlock::Create(ctx, "merge");
    builder.CreateCondBr(cond, cond_true, cond_false);
    builder.SetInsertPoint(cond_true);
    visit(if_br_stmt->get_comp_stmt());
    builder.CreateBr(merge);
    builder.SetInsertPoint(cond_false);

    return merge;
}

llvm::Value*CODEGEN::visit(LOGICAL_OR_EXPR_AST*expr)
{
    auto and_exprs = expr->get_and_exprs();
    llvm::Value*first = visit(and_exprs[0]);
    if (and_exprs.size() == 1) {
        return first;
    }
    auto not_first = builder.CreateICmpEQ(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
    auto log_first = builder.CreateICmpEQ(not_first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
    for (std::size_t i = 1; i < and_exprs.size(); i++) {
        auto not_second = builder.CreateICmpEQ(visit(and_exprs[i]), llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
        auto log_second = builder.CreateICmpEQ(not_second, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
        log_first = builder.CreateOr(log_first, log_second, "logortmp");
    }
    return builder.CreateAnd(log_first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
}

llvm::Value*CODEGEN::visit(LOGICAL_AND_EXPR_AST*expr)
{
    auto eq_exprs = expr->get_eq_exprs();
    llvm::Value*first = visit(eq_exprs[0]);
    if (eq_exprs.size() == 1) {
        return first;
    }
    auto not_first = builder.CreateICmpEQ(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
    auto log_first = builder.CreateICmpEQ(not_first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
    for (std::size_t i = 1; i < eq_exprs.size(); i++) {
        auto not_second = builder.CreateICmpEQ(visit(eq_exprs[i]), llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
        auto log_second = builder.CreateICmpEQ(not_second, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
        log_first = builder.CreateAnd(log_first, log_second, "logandtmp");
    }
    return builder.CreateAnd(log_first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
}

llvm::Value*CODEGEN::visit(EQ_EXPR_AST*expr)
{
    auto rel_exprs = expr->get_rel_exprs();
    llvm::Value*first = visit(rel_exprs[0].first);
    for (std::size_t i = 1; i < rel_exprs.size(); i++) {
        switch (rel_exprs[i - 1].second) {
        case DOMAIN_TAG::EQ:
            first = builder.CreateICmpEQ(first, visit(rel_exprs[i].first), "logortmp");
            first = builder.CreateAnd(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
            break;
        case DOMAIN_TAG::NEQ:
            first = builder.CreateICmpNE(first, visit(rel_exprs[i].first), "logortmp");
            first = builder.CreateAnd(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
            break;
        default:
            // ничего не делать.
            break;
        }
    }
    return first;
}

llvm::Value*CODEGEN::visit(REL_EXPR_AST*expr)
{
    auto add_exprs = expr->get_add_exprs();
    llvm::Value*first = visit(add_exprs[0].first);
    for (std::size_t i = 1; i < add_exprs.size(); i++) {
        switch (add_exprs[i - 1].second) {
        case DOMAIN_TAG::LESS:
            first = builder.CreateICmpSLT(first, visit(add_exprs[i].first), "lesstmp");
            first = builder.CreateAnd(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
            break;
        case DOMAIN_TAG::LESS_OR_EQ:
            first = builder.CreateICmpSLE(first, visit(add_exprs[i].first), "lesseqtmp");
            first = builder.CreateAnd(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
            break;
        case DOMAIN_TAG::MORE:
            first = builder.CreateICmpSGT(first, visit(add_exprs[i].first), "moretmp");
            first = builder.CreateAnd(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
            break;
        case DOMAIN_TAG::MORE_OR_EQ:
            first = builder.CreateICmpSGE(first, visit(add_exprs[i].first), "moreeqtmp");
            first = builder.CreateAnd(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 1, true)), "logandtmp");
            break;
        default:
            // TODO: throw exception.
            break;
        }
    }
    return first;
}

llvm::Value*CODEGEN::visit(ADD_EXPR_AST*expr)
{
    auto mult_exprs = expr->get_mult_exprs();
    llvm::Value*first = visit(mult_exprs[0].first);
    for (std::size_t i = 1; i < mult_exprs.size(); i++) {
        switch (mult_exprs[i - 1].second) {
        case DOMAIN_TAG::PLUS:
            first = builder.CreateAdd(first, visit(mult_exprs[i].first), "addtmp");
            break;
        case DOMAIN_TAG::MINUS:
            first = builder.CreateSub(first, visit(mult_exprs[i].first), "subtmp");
            break;
        default:
            // TODO: throw exception.
            break;
        }
    }
    return first;
}

llvm::Value*CODEGEN::visit(MULT_EXPR_AST*expr)
{
    auto left_unary_exprs = expr->get_left_unary_exprs();
    llvm::Value*first = visit(left_unary_exprs[0].first);
    for (std::size_t i = 1; i < left_unary_exprs.size(); i++) {
        switch (left_unary_exprs[i - 1].second) {
        case DOMAIN_TAG::MUL:
            first = builder.CreateMul(first, visit(left_unary_exprs[i].first), "multmp");
            break;
        case DOMAIN_TAG::DIV:
            first = builder.CreateSDiv(first, visit(left_unary_exprs[i].first), "divtmp");
            break;
        case DOMAIN_TAG::MOD:
            first = builder.CreateSRem(first, visit(left_unary_exprs[i].first), "modtmp");
            break;
        default:
            // TODO: throw exception.
            break;
        }
    }
    return first;
}

llvm::Value*CODEGEN::visit(LEFT_UNARY_EXPR_AST*expr)
{
    llvm::Value*first = visit(expr->get_first());
    if (expr->get_left_unary_op() != nullptr) {
        switch (*(expr->get_left_unary_op())) {
        case DOMAIN_TAG::PLUS:
            return builder.CreateAdd(llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), first, "unaryaddtmp");
        case DOMAIN_TAG::MINUS:
            return builder.CreateSub(llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), first, "unarysubtmp");
        case DOMAIN_TAG::LOGICAL_NOT:
            return builder.CreateICmpEQ(first, llvm::ConstantInt::get(ctx, llvm::APInt(32, 0, true)), "lognottmp");
        default:
            // TODO: throw exception;
            break;
        }
    }
    return first;
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

llvm::Module*CODEGEN::get_module()
{
    return module;
}

void CODEGEN::generate_obj(std::string objname)
{
    auto target_triple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    std::string error_str;
    auto Target = llvm::TargetRegistry::lookupTarget(target_triple, error_str);
    if (!Target) {
        std::cerr << error_str << std::endl;
        exit(-1);
    }

    llvm::TargetOptions opt;;
    auto target_platform = Target->createTargetMachine(
        target_triple,
        "generic",
        "",
        opt,
        llvm::Optional<llvm::Reloc::Model>());
    module->setDataLayout(target_platform->createDataLayout());
    module->setTargetTriple(target_triple);
    std::error_code EC;
    llvm::raw_fd_ostream dest(objname, EC, llvm::sys::fs::F_None);
    if (EC) {
        std::cerr << "Could not open file: " << EC.message() << std::endl;
        exit(-1);
    }

    llvm::legacy::PassManager pass;

    if (target_platform->addPassesToEmitFile(pass, dest, llvm::TargetMachine::CGFT_ObjectFile)) {
        std::cerr << "TargetMachine can't emit a file of this type" << std::endl;
        exit(1);
    }

    pass.run(*module);
    dest.flush();
}

CODEGEN::~CODEGEN()
{
}
