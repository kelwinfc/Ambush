#include "tests.hpp"

int main(int argc, char* argv[])
{
    graph g((char*)"test/graphs/g2.json");
    world w(&g);
    ambush amb(&w);
    noop np;
    
    int n = 4;
    agent target(n, &g, &w, &np, 5);
    vector<agent*> agents;
    for ( int i=0; i<n; i++ ){
        agent* a = new agent(i, &g, &w, &amb, 0);
        agents.push_back(a);
        agents[i]->set_target(&target);
        w.add_agent(agents[i]);
    }
    
    cout << w.ambush_rate(&target) << endl;
    return 0;
}