#include "controlflowgraph.hpp"

#include <fstream>
#include <algorithm>

void CFG::dfs(std::size_t cur_id, std::vector<std::size_t>&bbs_order)
{
    if (std::find(bbs_order.begin(), bbs_order.end(), cur_id) != bbs_order.end()) {
        return;
    }

    bbs_order.push_back(cur_id);
    
    
    auto bb = bbs[cur_id];
    for (std::size_t i = 0; i < bb->succs.size(); i++) {
        dfs(bbs[bb->succs[i]]->id, bbs_order);
    }
}

std::vector<std::size_t> CFG::compute_dominator_tree()
{
    std::vector<std::size_t> bbs_order;
    dfs(0, bbs_order);

    std::vector<std::size_t> doms(bbs.size(), bbs.size());

    for (std::size_t i = 0; i < bbs_order.size(); i++) {
        std::vector<std::size_t> cur_bbs_order;
        cur_bbs_order.push_back(bbs_order[i]);
        dfs(0, cur_bbs_order);
        for (std::size_t j = 0; j < doms.size(); j++) {
            if (std::find(cur_bbs_order.begin(), cur_bbs_order.end(), j) == cur_bbs_order.end()) {
                doms[j] = bbs_order[i];
            }
        }
    }

    doms[0] = 0;

    return doms;
}

std::set<std::size_t> CFG::compute_df(std::size_t cur_id, const std::vector<std::size_t>&doms, std::vector<std::set<std::size_t>>&df_s)
{
    std::set<std::size_t> s;
    auto bb = bbs[cur_id];
    
    // DF_local
    for (std::size_t i = 0; i < bb->succs.size(); i++) {
        if (doms[bb->succs[i]] != cur_id) {
            s.insert(bb->succs[i]);
        }
    }
    // DF_up
    for (std::size_t i = 0; i < doms.size(); i++) {
        if ((doms[i] == cur_id) && (i != cur_id)) {
            auto df_c = compute_df(i, doms, df_s);
            for (auto w : df_c) {
                bool flag = true;

                // if cur_id !dom w.
                std::size_t k = w;
                do {
                    if (doms[k] == cur_id) {
                        flag = false;
                        break;
                    }
                    k = doms[k];
                } while (doms[k] != 0);
                
                if (flag) {
                    s.insert(w);
                }
            }
        }
    }
    
    df_s[cur_id] = s;
    return s;
}

static void update_defsites_and_origs(
    SSA_OPERAND*ssa_op,
    int i,
    std::set<std::string>&variables,
    std::map<std::string, std::set<std::size_t>>&defsites,
    std::vector<std::set<std::string>>&origs)
{
    SSA_VARIABLE*ssa_var;
    if ((ssa_op != nullptr) && ((ssa_var = dynamic_cast<SSA_VARIABLE*>(ssa_op)) != nullptr) && (ssa_var->name[0] != '%')) {
        auto name = ssa_var->name;
        variables.insert(name);
        if (defsites.find(name) == defsites.end()) {
            defsites[name] = std::set<std::size_t>();
        }
        defsites[name].insert(i);
        origs[i].insert(name);
    }
}

static void update_version_in_expr(SSA_OPERAND*ssa_op, std::map<std::string, std::stack<std::size_t>>&stacks)
{
    SSA_VARIABLE*ssa_var;
    if ((ssa_op != nullptr) && ((ssa_var = dynamic_cast<SSA_VARIABLE*>(ssa_op)) != nullptr) && (ssa_var->name[0] != '%') && (ssa_var->version >= 0)) {
        auto name = ssa_var->name;
        ssa_var->version = stacks[name].top() - 1;
    }
}

void CFG::rename_variables(
    std::size_t cur_id,
    std::map<std::string, std::size_t>&counts,
    std::map<std::string, std::stack<std::size_t>>&stacks,
    const std::vector<std::size_t>&doms) {
    auto bb = bbs[cur_id];
    for (std::size_t i = 0; i < bb->tacs.size(); i++) {
        auto tac = bb->tacs[i];
        if (tac->op == THREE_ADDRESS_CODE_OPERATION::ALLOCA) {
            continue;
        }
        if (tac->op != THREE_ADDRESS_CODE_OPERATION::PHI) {
            update_version_in_expr(tac->src1, stacks);
            update_version_in_expr(tac->src2, stacks);
        }
        SSA_VARIABLE*ssa_var;
        if (((tac->op == THREE_ADDRESS_CODE_OPERATION::ASS) || (tac->op == THREE_ADDRESS_CODE_OPERATION::PHI)) &&
            ((ssa_var = dynamic_cast<SSA_VARIABLE*>(tac->dest)) != nullptr) &&
            (ssa_var->name[0] != '%')) {
            auto name = ssa_var->name;
            counts[name]++;
            auto version = counts[name];
            stacks[name].push(version);
            ssa_var->version = version - 1;
        }
    }
    for (std::size_t i = 0; i < bb->succs.size(); i++) {
        auto bb1 = bbs[bb->succs[i]];
        std::size_t j;
        for (j = 0; j < bb1->preds.size(); j++) {
            if (bb1->preds[j] == cur_id) {
                break;
            }
        }
        for (std::size_t k = 0; k < bb1->tacs.size(); k++) {
            auto tac = bb1->tacs[k];
            if (tac->op == THREE_ADDRESS_CODE_OPERATION::ALLOCA) {
                continue;
            }
            SSA_VARIABLE*ssa_var;
            if ((tac->op == THREE_ADDRESS_CODE_OPERATION::PHI) && ((ssa_var = dynamic_cast<SSA_VARIABLE*>(tac->dest)) != nullptr)) {
                auto name = ssa_var->name;
                if (j == 0) {
                    ssa_var = dynamic_cast<SSA_VARIABLE*>(tac->src1);
                    ssa_var->version = stacks[name].top() - 1;
                } else if (j == 1) {
                    ssa_var = dynamic_cast<SSA_VARIABLE*>(tac->src2);
                    ssa_var->version = stacks[name].top() - 1;
                }
            }
        }
    }
    for (std::size_t i = 0; i < doms.size(); i++) {
        if ((doms[i] == cur_id) && (i != cur_id)) {
            rename_variables(i, counts, stacks, doms);
        }
    }

    for (std::size_t i = 0; i < bb->tacs.size(); i++) {
        auto tac = bb->tacs[i];
        if (tac->op == THREE_ADDRESS_CODE_OPERATION::ALLOCA) {
            continue;
        }
        SSA_VARIABLE*ssa_var;
        if ((tac->dest != nullptr) && ((ssa_var = dynamic_cast<SSA_VARIABLE*>(tac->dest)) != nullptr) && (ssa_var->name[0] != '%')) {
            auto name = ssa_var->name;
            stacks[name].pop();
        }
    }
}

CFG::CFG(std::string funcname, std::vector<std::string> argnames, std::vector<BASIC_BLOCK*> bbs) :
    funcname(funcname), argnames(argnames), bbs(bbs), ssa(false) {}

void CFG::to_ssa()
{
    if (ssa) {
        return;
    }

    // Insert phi-functions.
    std::set<std::string> variables;
    std::map<std::string, std::set<std::size_t>> defsites;
    std::vector<std::set<std::string>> origs;

    // Building defsites and origs.
    for (std::size_t i = 0; i < bbs.size(); i++) {
        auto bb = bbs[i];
        for (std::size_t j = 0; j < bb->tacs.size(); j++) {
            origs.push_back(std::set<std::string>());
            auto tac = bb->tacs[j];
            update_defsites_and_origs(tac->dest, i, variables, defsites, origs);
        }
    }

    std::map<std::string, std::set<std::size_t>> phis;
    for (auto name : variables) {
        phis[name] = std::set<std::size_t>();
    }

    std::vector<std::set<std::size_t>> df_s(bbs.size());
    auto doms = compute_dominator_tree();
    compute_df(0, doms, df_s);

    for (auto a : variables) {
        auto&w = defsites[a];
        while (w.size() != 0) {
            auto it = w.begin();
            auto cur_id = *it;
            w.erase(w.begin());
            auto&df = df_s[cur_id];
            for (auto y : df) {
                if (phis[a].find(y) == phis[a].end()) {
                    auto bb = bbs[y];
                    bb->tacs.insert(bb->tacs.begin(),
                                    new THREE_ADDRESS_CODE(
                                                           new SSA_VARIABLE(a, 0),
                                                           THREE_ADDRESS_CODE_OPERATION::PHI,
                                                           new SSA_VARIABLE(a, 0),
                                                           new SSA_VARIABLE(a, 0)));
                    phis[a].insert(y);
                    if (origs[y].find(a) == origs[y].end()) {
                        w.insert(y);
                    }
                }
            }
        }
    }

    // Update variables versions.
    std::map<std::string, std::size_t> counts;
    std::map<std::string, std::stack<std::size_t>> stacks;

    for (auto a : variables) {
        counts[a] = 0;
        stacks[a] = std::stack<std::size_t>();
        stacks[a].push(0);
    }

    rename_variables(0, counts, stacks, doms);
    
    ssa = true;
}

std::ostream& operator<<(std::ostream&strm, CFG&cfg)
{
    strm << "define @" << cfg.funcname << "(";
    if (cfg.argnames.size() != 0) {
        for (std::size_t i = 0; i < cfg.argnames.size() - 1; i++) {
            strm << cfg.argnames[i] << ", ";
        }
        strm << cfg.argnames[cfg.argnames.size() - 1];
    }
    strm << ")" << std::endl;

    for (std::size_t i = 0; i < cfg.bbs.size(); i++) {
        strm << *cfg.bbs[i] << std::endl;
    }
    
    return strm;
}

static bool remove_graph_teminator(std::string fname)
{
    std::ifstream ifs(fname);
    if (!ifs.good()) {
        return false;
    }
  
    std::ofstream ofs; 
    ofs.open("/tmp/graph_temp.txt", std::ofstream::out); 
  
    std::string line;
    while (std::getline(ifs, line)) {
        if (line != "}") {
            ofs << line << std::endl;
        } else {
            ofs << std::endl;
        }
    }
  
    // closing output file 
    ofs.close(); 
  
    // closing input file 
    ifs.close(); 
  
    // remove the original file 
    remove(fname.c_str());
  
    // rename the file 
    rename("/tmp/graph_temp.txt", fname.c_str());
    return true;
}

void CFG::to_graphvis(std::string fname, unsigned int flags)
{
    std::ofstream gf;
    if (flags == REWRITE_FILE) {
        gf.open(fname);
        gf << "digraph \"" << "module" << "\" {" << std::endl;
    } else {
        if (remove_graph_teminator(fname)) {
            gf.open(fname, std::ios_base::out | std::ios_base::app);
        } else {
            gf.open(fname, std::ios_base::out | std::ios_base::app);
            gf << "digraph \"" << "module" << "\" {" << std::endl;
        }
    }
    gf << "\tsubgraph \"" << funcname << "\" {" << std::endl;
    // entry.
    gf << "\t\t" << funcname << "_entry" << " [" << std::endl;
    gf << "\t\t\tshape=Mdiamond," << std::endl;
    gf << "\t\t\tstyle=filled," << std::endl;
    gf << "\t\t\tfillcolor=white," << std::endl;
    gf << "\t\t\tlabel=\"ENTRY of " << funcname << "\"" << std::endl;
    gf << "\t\t];" << std::endl;
    gf << "\t\t" << funcname << "_entry" << " -> " << funcname << "_bb_" << bbs[0]->id << ";" << std::endl;
    // bbs.
    for (std::size_t i = 0; i < bbs.size(); i++) {
        auto bb = bbs[i];
        /*
        // You don't need it.
        for (std::size_t j = 0; j < bb->preds.size(); j++) {
            gf << "\t\t" << funcname << "_bb_" << bb->preds[j] << " -> " << funcname << "_bb_" << bb->id << ";" << std::endl;
        }
        */
        gf << "\t\t" << funcname << "_bb_" << bb->id << " [" << std::endl;
        gf << "\t\t\tshape=record," << std::endl;
        gf << "\t\t\tstyle=filled," << std::endl;
        gf << "\t\t\tfillcolor=lightgrey," << std::endl;
        gf << "\t\t\tlabel=\"{" << std::endl;
        gf << "\t\t\t\t" << bb->id << " |" << std::endl;
        for (std::size_t j = 0; j < bb->tacs.size(); j++) {
            auto tac = bb->tacs[j];
            if (tac->op != THREE_ADDRESS_CODE_OPERATION::CONDBR) {
                gf << "\t\t\t\t" << *(tac) << "\\l\\" << std::endl;
            } else {
                gf << "\t\t\t\t" << *(tac) << " label %" << bb->succs[0] << ", label %" << bb->succs[1]  << "\\l\\" << std::endl;
            }
        }
        if (bb->succs.size() == 1) {
            gf << "\t\t\t\t" << "br label %" << bb->succs[0] << "\\l\\" << std::endl;
        }
        gf << "\t\t\t}\"" << std::endl;
        gf << "\t\t];" << std::endl;
        for (std::size_t j = 0; j < bb->succs.size(); j++) {
            gf << "\t\t" << funcname << "_bb_" << bb->id << " -> " << funcname << "_bb_" << bb->succs[j] << ";" << std::endl;
        }
    }
    // exit.
    gf << "\t\t" << funcname << "_exit" << " [" << std::endl;
    gf << "\t\t\tshape=Mdiamond," << std::endl;
    gf << "\t\t\tstyle=filled," << std::endl;
    gf << "\t\t\tfillcolor=white," << std::endl;
    gf << "\t\t\tlabel=\"EXIT of " << funcname << "\"" << std::endl;
    gf << "\t\t];" << std::endl;
    gf << "\t\t" << funcname << "_bb_" << bbs[bbs.size() - 1]->id << " -> " << funcname << "_exit" << ";" << std::endl;
    gf << "\t}" << std::endl;
    gf << "}" << std::endl;
    gf.close();
}

CFG::~CFG()
{
    for (std::size_t i = 0; i < bbs.size(); i++) {
        delete bbs[i];
    }
}
