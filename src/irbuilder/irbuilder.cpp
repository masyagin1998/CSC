#include "irbuilder.hpp"
#include "basicblock.hpp"

const std::string IR_BUILDER::eqtmp_str =     "%eqtmp";
const std::string IR_BUILDER::netmp_str =     "%netmp";
const std::string IR_BUILDER::logandtmp_str = "%logandtmp";
const std::string IR_BUILDER::logortmp_str =  "%logortmp";
const std::string IR_BUILDER::lognottmp_str = "%lognottmp";
const std::string IR_BUILDER::lesstmp_str =   "%lesstmp";
const std::string IR_BUILDER::moretmp_str =   "%moretmp";
const std::string IR_BUILDER::lesseqtmp_str = "%lesseqtmp";
const std::string IR_BUILDER::moreeqtmp_str = "%moreeqtmp";
const std::string IR_BUILDER::addtmp_str =    "%addtmp";
const std::string IR_BUILDER::subtmp_str =    "%subtmp";
const std::string IR_BUILDER::multmp_str =    "%multmp";
const std::string IR_BUILDER::sdivtmp_str =   "%sdivtmp";
const std::string IR_BUILDER::smodtmp_str =   "%smodtmp";

IR_BUILDER::IR_BUILDER(FUNC_AST*func) :
    cfg(nullptr),
    logandtmp_version(0),
    logortmp_version(0),
    lognottmp_version(0),
    eqtmp_version(0),
    netmp_version(0),
    lesstmp_version(0),
    moretmp_version(0),
    lesseqtmp_version(0),
    moreeqtmp_version(0),
    addtmp_version(0),
    subtmp_version(0),
    multmp_version(0),
    sdivtmp_version(0),
    smodtmp_version(0)
{
    std::vector<BASIC_BLOCK*> bbs;
    visit_block(func, bbs);

    std::string funcname = func->get_ident()->get_ident();
    auto args_list_tree = func->get_args_list()->get_args_list();
    std::vector<std::string> funcargs;
    for (std::size_t i = 0; i < args_list_tree.size(); i++) {
        funcargs.push_back(args_list_tree[i]->get_ident()->get_ident());
    }
    cfg = new CFG(funcname, funcargs, bbs);
}

#define APPEND_BASIC_BLOCK if (tacs.size() != 0) { \
    if ((bbs.size() == 0) || (bbs.back()->tacs.size() != 0)) {  \
        std::vector<std::size_t> preds;                         \
        if (bbs.size() != 0) {                                  \
            preds.push_back(bbs.back()->id);                    \
        }                                                       \
        std::vector<std::size_t> succs;                                 \
        auto bb =  new BASIC_BLOCK(preds, bbs.size(), succs, tacs);     \
        if (bbs.size() != 0) {                                          \
            bbs.back()->succs.push_back(bb->id);                        \
        }                                                               \
        bbs.push_back(bb);                                              \
    } else {                                                            \
        bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end()); \
    }                                                                   \
    tacs.clear();                                                       \
    }                                                                   \

#define ITERATE_STMTS for (std::size_t i = 0; i < stmts.size(); i++) { \
    IF_STMT_AST*if_stmt;                                               \
    WHILE_STMT_AST*while_stmt;                                          \
    if ((if_stmt = dynamic_cast<IF_STMT_AST*>(stmts[i]->get_stmt())) != nullptr) { \
        APPEND_BASIC_BLOCK;                                             \
        visit_block(if_stmt, bbs);                                      \
    } else if ((while_stmt = dynamic_cast<WHILE_STMT_AST*>(stmts[i]->get_stmt())) != nullptr) { \
        APPEND_BASIC_BLOCK;                                             \
        visit_block(while_stmt, bbs);                                   \
    } else {                                                            \
        visit_stmt(stmts[i], tacs);                                     \
    }                                                                   \
    }                                                                   \
    
void IR_BUILDER::visit_block(FUNC_AST*func, std::vector<BASIC_BLOCK*>&bbs)
{
    std::vector<THREE_ADDRESS_CODE*> tacs;

    // args.
    auto args_list = func->get_args_list()->get_args_list();
    for (std::size_t i = 0; i < args_list.size(); i++) {
        auto name = args_list[i]->get_ident()->get_ident();
        tacs.push_back(new THREE_ADDRESS_CODE(nullptr, THREE_ADDRESS_CODE_OPERATION::ALLOCA, new SSA_VARIABLE(name, 0), nullptr));
        tacs.push_back(new THREE_ADDRESS_CODE(new SSA_VARIABLE(name, 0), THREE_ADDRESS_CODE_OPERATION::ASS, new SSA_VARIABLE(name, -1), nullptr));
    }

    auto body = func->get_body();

    // body.
    // decls.
    auto decls = body->get_decls();
    for (std::size_t i = 0; i < decls.size(); i++) {
        visit_inblock(decls[i], tacs);
    }
    // stmts.
    auto stmts = body->get_stmts();
    ITERATE_STMTS;

    if (tacs.size() != 0) {
        if ((bbs.size() == 0) || (bbs.back()->tacs.size() != 0)) {
            std::vector<std::size_t> preds;
            if (bbs.size() != 0) {
                preds.push_back(bbs.back()->id);
            }
            std::vector<std::size_t> succs;
            auto bb =  new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
            if (bbs.size() != 0) {
                bbs.back()->succs.push_back(bb->id);
            }
            bbs.push_back(bb);
        } else {
            bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end());
        }
    }
}

void IR_BUILDER::visit_block(WHILE_STMT_AST*while_stmt, std::vector<BASIC_BLOCK*>&bbs)
{
    std::vector<THREE_ADDRESS_CODE*> tacs;
    
    // while_loop.
    auto ssa_op = visit_expr(while_stmt->get_logical(), tacs);
    tacs.push_back(new THREE_ADDRESS_CODE(nullptr, THREE_ADDRESS_CODE_OPERATION::CONDBR, ssa_op, nullptr));
    std::vector<std::size_t> preds;
    if (bbs.size() != 0) {
        preds.push_back(bbs.back()->id);
    }
    std::vector<std::size_t> succs;
    BASIC_BLOCK*while_loop = nullptr;
    std::size_t while_loop_index;
    if ((bbs.size() == 0) || (bbs.back()->tacs.size() != 0)) {
        while_loop = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
        if (bbs.size() != 0) {
            bbs.back()->succs.push_back(while_loop->id);
        }
        bbs.push_back(while_loop);
        while_loop_index = bbs.size() - 1;
    } else {
        bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end());
        while_loop = bbs.back();
        while_loop_index = bbs.size() - 1;
    }

    // body.
    tacs.clear();
    auto body = while_stmt->get_comp_stmt();
    // decls.
    auto decls = body->get_decls();
    for (std::size_t i = 0; i < decls.size(); i++) {
        visit_inblock(decls[i], tacs);
    }
    // stmts.
    auto stmts = body->get_stmts();
    ITERATE_STMTS;

    if (tacs.size() != 0) { // end of while body is basic sequence of stmts.
        if ((bbs.size() == 0) || (bbs.back()->tacs.size() != 0)) {
            std::vector<std::size_t> preds;
            if (bbs.size() != 0) {
                preds.push_back(bbs.back()->id);
            }
            std::vector<std::size_t> succs;
            succs.push_back(while_loop_index);
            auto bb = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
            if (bbs.size() != 0) {
                bbs.back()->succs.push_back(bb->id);
            }
            while_loop->preds.push_back(bb->id);
            bbs.push_back(bb);
        } else {
            bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end());
            bbs.back()->succs.push_back(while_loop_index);
            while_loop->preds.push_back(bbs.back()->id);
        }
        tacs.clear();
    } else if (bbs.size() == while_loop_index + 1) { // Empty while body.
        preds.clear();
        preds.push_back(bbs.back()->id);
        succs.clear();
        succs.push_back(while_loop->id);
        auto bb = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
        bbs.back()->succs.push_back(bb->id);
        while_loop->preds.push_back(bb->id);
        bbs.push_back(bb);
        tacs.clear();
    } else {  // end of while body is while or if.
        bbs.back()->succs.push_back(while_loop->id);
        while_loop->preds.push_back(bbs.back()->id);
    }

    // while_end.
    preds.clear();
    preds.push_back(while_loop->id);
    succs.clear();
    auto while_end = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
    bbs.push_back(while_end);
    bbs[while_loop_index]->succs.push_back(while_end->id);
}

void IR_BUILDER::visit_block(IF_STMT_AST*if_stmt, std::vector<BASIC_BLOCK*>&bbs)
{
    auto if_branch = if_stmt->get_first();

    std::vector<THREE_ADDRESS_CODE*> tacs;
    
    // cond_expr.
    auto ssa_op = visit_expr(if_branch->get_logical(), tacs);
    tacs.push_back(new THREE_ADDRESS_CODE(nullptr, THREE_ADDRESS_CODE_OPERATION::CONDBR, ssa_op, nullptr));
    std::vector<std::size_t> preds;
    if (bbs.size() != 0) {
        preds.push_back(bbs.back()->id);
    }
    std::vector<std::size_t> succs;
    BASIC_BLOCK*cond_expr = nullptr;
    std::size_t cond_expr_index;
    if ((bbs.size() == 0) || ((bbs.back()->tacs.size() != 0) && (bbs.back()->tacs.back()->op == THREE_ADDRESS_CODE_OPERATION::CONDBR))) {
        cond_expr = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
        if (bbs.size() != 0) {
            bbs.back()->succs.push_back(cond_expr->id);
        }
        bbs.push_back(cond_expr);
        cond_expr_index = bbs.size() - 1;
    } else {
        bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end());
        cond_expr = bbs.back();
        cond_expr_index = bbs.size() - 1;
    }

    // true_stmt.
    tacs.clear();
    auto true_stmt = if_branch->get_comp_stmt();
    // decls.
    auto decls = true_stmt->get_decls();
    for (std::size_t i = 0; i < decls.size(); i++) {
        visit_inblock(decls[i], tacs);
    }
    // stmts.
    auto stmts = true_stmt->get_stmts();
    ITERATE_STMTS;

    if (tacs.size() != 0) { // end of true_stmt body is basic sequence of stmts.
        if ((bbs.size() == 0) || (bbs.back()->tacs.size() != 0)) {
            std::vector<std::size_t> preds;
            if (bbs.size() != 0) {
                preds.push_back(bbs.back()->id);
            }
            std::vector<std::size_t> succs;
            auto bb = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
            if (bbs.size() != 0) {
                bbs.back()->succs.push_back(bb->id);
            }
            bbs.push_back(bb);
        } else {
            bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end());
        }
        tacs.clear();
    } else if (bbs.size() == cond_expr_index + 1) { // Empty true_stmt body.
        preds.clear();
        preds.push_back(bbs.back()->id);
        succs.clear();
        auto bb = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
        bbs.back()->succs.push_back(bb->id);
        bbs.push_back(bb);
        tacs.clear();
    } else {  // end of true_stmt body is while or if.
        // Maybe do nothing.
    }

    auto true_stmt_merge = bbs.back();

    auto false_stmt = if_stmt->get_second();
    if (false_stmt == nullptr) {
        // merge with empty false_stmt.
        preds.clear();
        preds.push_back(true_stmt_merge->id);
        preds.push_back(cond_expr_index);
        succs.clear();
        auto merge = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
        true_stmt_merge->succs.push_back(merge->id);
        cond_expr->succs.push_back(merge->id);
        bbs.push_back(merge);
        return;
    }

    // false_stmt.
    tacs.clear();
    preds.clear();
    preds.push_back(cond_expr_index);
    succs.clear();
    auto false_stmt_begin = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
    cond_expr->succs.push_back(false_stmt_begin->id);
    bbs.push_back(false_stmt_begin);
    auto false_stmt_begin_index = bbs.size() - 1;
    
    IF_STMT_AST*false_stmt_if;
    COMPOUND_STMT_AST*false_stmt_comp;
    if ((false_stmt_if = dynamic_cast<IF_STMT_AST*>(false_stmt)) != nullptr) {
        visit_block(false_stmt_if, bbs);
    } else if ((false_stmt_comp = dynamic_cast<COMPOUND_STMT_AST*>(false_stmt)) != nullptr) {
        auto stmts = false_stmt_comp->get_stmts();
        ITERATE_STMTS;
        if (tacs.size() != 0) { // end of false_stmt body is basic sequence of stmts.
            if ((bbs.size() == 0) || (bbs.back()->tacs.size() != 0)) {
                std::vector<std::size_t> preds;
                if (bbs.size() != 0) {
                    preds.push_back(bbs.back()->id);
                }
                std::vector<std::size_t> succs;
                auto bb = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
                if (bbs.size() != 0) {
                    bbs.back()->succs.push_back(bb->id);
                }
                bbs.push_back(bb);
            } else {
                bbs.back()->tacs.insert(bbs.back()->tacs.end(), tacs.begin(), tacs.end());
            }
            tacs.clear();
        } else if (bbs.size() == false_stmt_begin_index + 1) { // Empty true_stmt body.
            // Maybe do nothing.
        } else {  // end of true_stmt body is while or if.
            // Maybe do nothing.
        }
    } else {
        // TODO: throw exception.
    }

    BASIC_BLOCK*false_stmt_merge = bbs.back();

    // merge.
    preds.clear();
    preds.push_back(true_stmt_merge->id);
    preds.push_back(false_stmt_merge->id);
    succs.clear();
    tacs.clear();
    BASIC_BLOCK*merge = new BASIC_BLOCK(preds, bbs.size(), succs, tacs);
    true_stmt_merge->succs.push_back(merge->id);
    false_stmt_merge->succs.push_back(merge->id);
    bbs.push_back(merge);
}

int IR_BUILDER::visit_stmt(STMT_AST*stmt, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    // WHILE and IF stmts are checked on higher level.
    ASS_STMT_AST*ass_stmt;
    COMPOUND_STMT_AST*comp_stmt;
    RET_STMT_AST*ret_stmt;

    if ((ass_stmt = dynamic_cast<ASS_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        visit_inblock(ass_stmt, tacs);
    } else if ((comp_stmt = dynamic_cast<COMPOUND_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        auto decls = comp_stmt->get_decls();
        for (std::size_t i = 0; i < decls.size(); i++) {
            visit_inblock(decls[i], tacs);
        }
        auto stmts = comp_stmt->get_stmts();
        for (std::size_t i = 0; i < stmts.size(); i++) {
            if (visit_stmt(stmts[i], tacs) == -1) {
                break;
            }
        }
    } else if ((ret_stmt = dynamic_cast<RET_STMT_AST*>(stmt->get_stmt())) != nullptr) {
        visit_inblock(ret_stmt, tacs);
        return -1;
    }

    return 0;
}

void IR_BUILDER::visit_inblock(DECL_AST*decl, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    SSA_VARIABLE*ssa_var = new SSA_VARIABLE(decl->get_ident()->get_ident(), 0);
    tacs.push_back(new THREE_ADDRESS_CODE(nullptr, THREE_ADDRESS_CODE_OPERATION::ALLOCA, ssa_var, nullptr));
    if (decl->get_expr() != nullptr) {
        ssa_var = new SSA_VARIABLE(decl->get_ident()->get_ident(), 0);
        tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::ASS, visit_expr(decl->get_expr(), tacs), nullptr));
    }
}

void IR_BUILDER::visit_inblock(ASS_STMT_AST*ass_stmt, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto ssa_var = new SSA_VARIABLE(ass_stmt->get_ident()->get_ident(), 0);
    tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::ASS, visit_expr(ass_stmt->get_expr(), tacs), nullptr));
}

void IR_BUILDER::visit_inblock(RET_STMT_AST*ret_stmt, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto ssa_op = visit_expr(ret_stmt->get_expr(), tacs);
    tacs.push_back(new THREE_ADDRESS_CODE(nullptr, THREE_ADDRESS_CODE_OPERATION::RET, ssa_op, nullptr));
}

SSA_OPERAND*IR_BUILDER::visit_expr(LOGICAL_OR_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto and_exprs = expr->get_and_exprs();
    auto first = visit_expr(and_exprs[0], tacs);
    if (and_exprs.size() == 1) {
        return first;
    }
    for (std::size_t i = 1; i < and_exprs.size(); i++) {
        auto second = visit_expr(and_exprs[i], tacs);
        auto ssa_var = new SSA_VARIABLE(logortmp_str, logortmp_version++);
        tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::LOGICAL_OR, first, second));
        first = new SSA_VARIABLE(ssa_var->name, ssa_var->version);
    }
    return first;
}

SSA_OPERAND*IR_BUILDER::visit_expr(LOGICAL_AND_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto eq_exprs = expr->get_eq_exprs();
    auto first = visit_expr(eq_exprs[0], tacs);
    if (eq_exprs.size() == 1) {
        return first;
    }
    for (std::size_t i = 1; i < eq_exprs.size(); i++) {
        auto second = visit_expr(eq_exprs[i], tacs);
        auto ssa_var = new SSA_VARIABLE(logandtmp_str, logandtmp_version++);
        tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::LOGICAL_AND, first, second));
        first = new SSA_VARIABLE(ssa_var->name, ssa_var->version);
    }
    return first;
}

SSA_OPERAND*IR_BUILDER::visit_expr(EQ_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto rel_exprs = expr->get_rel_exprs();
    auto first = visit_expr(rel_exprs[0].first, tacs);
    if (rel_exprs.size() == 1) {
        return first;
    }
    for (std::size_t i = 1; i < rel_exprs.size(); i++) {
        auto second = visit_expr(rel_exprs[i].first, tacs);
        SSA_VARIABLE*ssa_var;
        switch (rel_exprs[i - 1].second) {
        case DOMAIN_TAG::EQ:
            ssa_var = new SSA_VARIABLE(eqtmp_str, eqtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::EQ, first, second));
            break;
        case DOMAIN_TAG::NEQ:
            ssa_var = new SSA_VARIABLE(netmp_str, netmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::NE, first, second));
            break;
        default:
            // do nothing.
            break;
        }
        first = new SSA_VARIABLE(ssa_var->name, ssa_var->version);
    }
    return first;
}

SSA_OPERAND*IR_BUILDER::visit_expr(REL_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto add_exprs = expr->get_add_exprs();
    auto first = visit_expr(add_exprs[0].first, tacs);
    if (add_exprs.size() == 1) {
        return first;
    }
    for (std::size_t i = 1; i < add_exprs.size(); i++) {
        auto second = visit_expr(add_exprs[i].first, tacs);
        SSA_VARIABLE*ssa_var;
        switch (add_exprs[i - 1].second) {
        case DOMAIN_TAG::LESS:
            ssa_var = new SSA_VARIABLE(lesstmp_str, lesstmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::LESS, first, second));
            break;
        case DOMAIN_TAG::MORE:
            ssa_var = new SSA_VARIABLE(moretmp_str, moretmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::MORE, first, second));
            break;
        case DOMAIN_TAG::LESS_OR_EQ:
            ssa_var = new SSA_VARIABLE(lesseqtmp_str, lesseqtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::LESS_OR_EQ, first, second));
            break;
        case DOMAIN_TAG::MORE_OR_EQ:
            ssa_var = new SSA_VARIABLE(moreeqtmp_str, moreeqtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::MORE_OR_EQ, first, second));
            break;
        default:
            // do nothing.
            break;
        }
        first = new SSA_VARIABLE(ssa_var->name, ssa_var->version);
    }
    return first;
}

SSA_OPERAND*IR_BUILDER::visit_expr(ADD_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto mult_exprs = expr->get_mult_exprs();
    auto first = visit_expr(mult_exprs[0].first, tacs);
    if (mult_exprs.size() == 1) {
        return first;
    }
    for (std::size_t i = 1; i < mult_exprs.size(); i++) {
        auto second = visit_expr(mult_exprs[i].first, tacs);
        SSA_VARIABLE*ssa_var;
        switch (mult_exprs[i - 1].second) {
        case DOMAIN_TAG::PLUS:
            ssa_var = new SSA_VARIABLE(addtmp_str, addtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::PLUS, first, second));
            break;
        case DOMAIN_TAG::MINUS:
            ssa_var = new SSA_VARIABLE(subtmp_str, subtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::MINUS, first, second));
            break;
        default:
            // do nothing.
            break;
        }
        first = new SSA_VARIABLE(ssa_var->name, ssa_var->version);
    }
    return first;
}

SSA_OPERAND*IR_BUILDER::visit_expr(MULT_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto left_unary_exprs = expr->get_left_unary_exprs();
    auto first = visit_expr(left_unary_exprs[0].first, tacs);
    if (left_unary_exprs.size() == 1) {
        return first;
    }
    for (std::size_t i = 1; i < left_unary_exprs.size(); i++) {
        auto second = visit_expr(left_unary_exprs[i].first, tacs);
        SSA_VARIABLE*ssa_var;
        switch (left_unary_exprs[i - 1].second) {
        case DOMAIN_TAG::MUL:
            ssa_var = new SSA_VARIABLE(multmp_str, multmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::MUL, first, second));
            break;
        case DOMAIN_TAG::DIV:
            ssa_var = new SSA_VARIABLE(sdivtmp_str, sdivtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::SDIV, first, second));
            break;
        case DOMAIN_TAG::MOD:
            ssa_var = new SSA_VARIABLE(smodtmp_str, smodtmp_version++);
            tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::SMOD, first, second));
            break;
        default:
            // do nothing.
            break;
        }
        first = new SSA_VARIABLE(ssa_var->name, ssa_var->version);
    }
    return first;
}

SSA_OPERAND*IR_BUILDER::visit_expr(LEFT_UNARY_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    auto first = visit_expr(expr->get_first(), tacs);
    if (expr->get_left_unary_op() == nullptr) {
        return first;
    }
    switch (*(expr->get_left_unary_op())) {
    case DOMAIN_TAG::PLUS: {
        auto ssa_int = new SSA_INTEGER(0);
        auto ssa_var = new SSA_VARIABLE(addtmp_str, addtmp_version++);
        tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::PLUS, ssa_int, first));
        return new SSA_VARIABLE(ssa_var->name, ssa_var->version);;
    }
    case DOMAIN_TAG::MINUS: {
        auto ssa_int = new SSA_INTEGER(0);
        auto ssa_var = new SSA_VARIABLE(subtmp_str, subtmp_version++);
        tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::MINUS, ssa_int, first));
        return new SSA_VARIABLE(ssa_var->name, ssa_var->version);;
    }
    case DOMAIN_TAG::LOGICAL_NOT: {
        auto ssa_var = new SSA_VARIABLE(lognottmp_str, lognottmp_version++);
        tacs.push_back(new THREE_ADDRESS_CODE(ssa_var, THREE_ADDRESS_CODE_OPERATION::LOGICAL_NOT, first, nullptr));
        return new SSA_VARIABLE(ssa_var->name, ssa_var->version);;
        break;
    }
    default:
        // do nothing.
        break;
        
    }
}

SSA_OPERAND*IR_BUILDER::visit_expr(PRIMARY_EXPR_AST*expr, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    IDENT_AST*ident;
    INTEGER_AST*integer;
    LOGICAL_OR_EXPR_AST*inner_expr;

    if ((ident = dynamic_cast<IDENT_AST*>(expr->get_primary_expr())) != nullptr) {
        return visit_expr(ident, tacs);
    } else if ((integer = dynamic_cast<INTEGER_AST*>(expr->get_primary_expr())) != nullptr) {
        return visit_expr(integer, tacs);
    } else if ((inner_expr = dynamic_cast<LOGICAL_OR_EXPR_AST*>(expr->get_primary_expr())) != nullptr) {
        return visit_expr(inner_expr, tacs);
    } else {
        // do nothing.
    }
}

SSA_OPERAND*IR_BUILDER::visit_expr(IDENT_AST*ident, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    return new SSA_VARIABLE(ident->get_ident(), 0);
}

SSA_OPERAND*IR_BUILDER::visit_expr(INTEGER_AST*integer, std::vector<THREE_ADDRESS_CODE*>&tacs)
{
    return new SSA_INTEGER(integer->get_integer());
}

CFG*IR_BUILDER::get_cfg()
{
    return cfg;
}
