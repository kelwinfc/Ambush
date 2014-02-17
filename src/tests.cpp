#include "tests.hpp"

void eval_behavior(world& w, vector<agent*>& agents, agent& target,
                   behavior* b, string name)
{
    int n = (int)agents.size();
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behavior(b);
    }
    w.clear_paths();
    
    const clock_t begin_time = clock();
    w.compute_paths(&target);
    const clock_t end_time = clock();
    
    printf("%s: %0.4f %6.2f %6.2f %10.2f\n",
           name.c_str(),
           w.ambush_rate(&target),
           w.increment_rate(&target),
           w.graph_coverage(),
           float( end_time - begin_time ) * 1000.0 / 
               (float)( CLOCKS_PER_SEC * agents.size() )
          );
}

int main(int argc, char* argv[])
{
    argc--;
    argv++;
    
    int n = 10;
    if ( argc > 1 ){
        sscanf(argv[1], "%d", &n);
    }
    
    char* graph_name = (char*)"test/graphs/level2.json";
    if ( argc > 0 ){
        graph_name = argv[0];
    }
    graph g(graph_name);
    
    world w(&g);
    noop np(&w);
    randomized_dfs dfs(&w);
    a_star ast(&w);
    ambush amb(&w);
    priority_ambush pamb(&w);
    self_adaptive_r_ambush sar(&w);
    
    agent target(n, &g, &w, &np, 5);
    
    vector<agent*> agents;
    for ( int i=0; i<n; i++ ){
        agent* a = new agent(i, &g, &w, &np, min(i,g.num_vertex()-1)/*0*/);
        agents.push_back(a);
        agents[i]->set_target(&target);
        w.add_agent(agents[i]);
    }
    
    printf("Algorithm: Ambush-Rate Path-Increment(%%) Graph-Coverage(%%) Time(ms)\n");
    
    eval_behavior(w, agents, target, &ast,  "         A*");
    eval_behavior(w, agents, target, &dfs,  "        DFS");
    eval_behavior(w, agents, target, &amb,  "    A*mbush");
    eval_behavior(w, agents, target, &pamb, "  P-A*mbush");
    eval_behavior(w, agents, target, &sar,  "SAR-A*mbush");
    
    return 0;
}
