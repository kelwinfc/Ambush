#include "tests.hpp"

void eval_behavior(world& w, vector<agent*>& agents, agent& target,
                   behavior* b, string name)
{
    int n = (int)agents.size();
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behavior(b);
    }
    w.clear_paths();
    w.compute_paths(&target);
    
    printf("%s: %0.4f %3.2f%% %3.2f%%\n",
           name.c_str(),
           w.ambush_rate(&target),
           w.increment_rate(&target),
           w.graph_coverage()
          );
}
int main(int argc, char* argv[])
{
    graph g((char*)"test/graphs/g2.json");
    world w(&g);
    noop np(&w);
    randomized_dfs dfs(&w);
    a_star ast(&w);
    ambush amb(&w);
    priority_ambush pamb(&w);
    self_adaptive_r_ambush sar(&w);
    
    int n = 4;
    agent target(n, &g, &w, &np, 5);
    
    vector<agent*> agents;
    for ( int i=0; i<n; i++ ){
        agent* a = new agent(i, &g, &w, &np, 0);
        agents.push_back(a);
        agents[i]->set_target(&target);
        w.add_agent(agents[i]);
    }
    
    printf("Algorithm: Ambush-Rate Path-Increment(%%) Graph-Coverage(%%)\n");
    
    eval_behavior(w, agents, target, &ast,  "         A*");
    eval_behavior(w, agents, target, &dfs,  "        DFS");
    eval_behavior(w, agents, target, &amb,  "    A*mbush");
    eval_behavior(w, agents, target, &pamb, "  P-A*mbush");
    eval_behavior(w, agents, target, &sar,  "SAR-A*mbush");
    
    return 0;
}
