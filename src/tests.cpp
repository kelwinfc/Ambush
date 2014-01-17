#include "tests.hpp"

int main(int argc, char* argv[])
{
    graph g((char*)"test/graphs/g2.json");
    world w(&g);
    noop np(&w);
    randomized_dfs dfs(&w);
    a_star ast(&w);
    ambush amb(&w);
    priority_ambush pamb(&w);
    
    int n = 4;
    agent target(n, &g, &w, &np, 5);
    
    vector<agent*> agents;
    for ( int i=0; i<n; i++ ){
        agent* a = new agent(i, &g, &w, &np, 0);
        agents.push_back(a);
        agents[i]->set_target(&target);
        w.add_agent(agents[i]);
    }
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behaviour(&dfs);
    }
    w.clear_paths();
    w.compute_paths(&target);
    cout << "DFS: " << w.ambush_rate(&target)
         << ", " << w.increment_rate(&target) << "%"
         << endl;
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behaviour(&ast);
    }
    w.clear_paths();
    w.compute_paths(&target);
    cout << "A*: " << w.ambush_rate(&target)
         << ", " << w.increment_rate(&target) << "%"
         << endl;
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behaviour(&amb);
    }
    w.clear_paths();
    w.compute_paths(&target);
    cout << "A*mbush: " << w.ambush_rate(&target)
         << ", " << w.increment_rate(&target) << "%"
         << endl;
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behaviour(&pamb);
    }
    w.clear_paths();
    w.compute_paths(&target);
    cout << "P-A*mbush: " << w.ambush_rate(&target)
         << ", " << w.increment_rate(&target) << "%"
         << endl;
    
    return 0;
}
