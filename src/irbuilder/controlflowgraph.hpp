#ifndef CONTROL_FLOW_GRAPH_HPP_INCLUDED
#define CONTROL_FLOW_GRAPH_HPP_INCLUDED

#include "lexer.hpp"

#include "funcast.hpp"

#include "basicblock.hpp"

#include <vector>
#include <set>
#include <map>
#include <stack>

#define REWRITE_FILE 0
#define APPEND_FILE  1

class CFG
{
  private:
    std::string funcname;
    std::vector<std::string> argnames;
    std::vector<BASIC_BLOCK*> bbs;
    bool ssa;
 
    void dfs(std::size_t cur_id, std::vector<std::size_t>&bbs_order);
    std::vector<std::size_t> compute_dominator_tree();
    std::set<std::size_t> compute_df(std::size_t cur_id, const std::vector<std::size_t>&doms, std::vector<std::set<std::size_t>>&df_s);
    void rename_variables(
        std::size_t cur_id,
        std::map<std::string, std::size_t>&counts,
        std::map<std::string, std::stack<std::size_t>>&stacks,
        const std::vector<std::size_t>&doms);

  public:
    CFG(std::string funcname, std::vector<std::string> argnames, std::vector<BASIC_BLOCK*> bbs);

    void to_ssa();

    friend std::ostream& operator<<(std::ostream&strm, CFG&cfg);

    void to_graphvis(std::string fname, unsigned int flags);
    
    ~CFG();
};

#endif  // CONTROL_FLOW_GRAPH_HPP_INCLUDED
