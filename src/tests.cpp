#include "tests.hpp"

int main(int argc, char* argv[])
{
    graph g((char*)"test/graphs/g2.json");
    world w(&g);
    ambush amb(&w);
    a_star ast(&w);
    
    noop np;
    
    int n = 4;
    agent target(n, &g, &w, &np, 5);
    
    vector<agent*> astar_agents;
    for ( int i=0; i<n; i++ ){
        agent* a = new agent(i, &g, &w, &ast, 0);
        astar_agents.push_back(a);
        astar_agents[i]->set_target(&target);
        w.add_agent(astar_agents[i]);
    }
    cout << w.ambush_rate(&target) << endl;
    
    vector<agent*> ambush_agents;
    for ( int i=0; i<n; i++ ){
        agent* a = new agent(i, &g, &w, &amb, 0);
        ambush_agents.push_back(a);
        ambush_agents[i]->set_target(&target);
        w.add_agent(ambush_agents[i]);
    }
    cout << w.ambush_rate(&target) << endl;
    
    return 0;
}