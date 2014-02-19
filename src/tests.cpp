#include "tests.hpp"

void eval_behavior(world& w, vector<agent*>& agents, agent& target,
                   behavior* b, vector<float>& results)
{
    int n = (int)agents.size();
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behavior(b);
    }
    w.clear_paths();
    
    const clock_t begin_time = clock();
    w.compute_paths(&target);
    const clock_t end_time = clock();

    results[0] += w.ambush_rate_relaxed(&target);
    results[1] += w.increment_rate(&target);
    results[2] += w.graph_coverage();
    results[3] += float( end_time - begin_time ) * 1000.0 /
               (float)( CLOCKS_PER_SEC * agents.size() );
    /*
    printf("%s: %0.4f %6.2f %6.2f %10.2f\n",
           name.c_str(),
           w.ambush_rate(&target),
           w.increment_rate(&target),
           w.graph_coverage(),
           float( end_time - begin_time ) * 1000.0 / 
               (float)( CLOCKS_PER_SEC * agents.size() )
          );
    */
}

int main(int argc, char* argv[])
{
    argc--;
    argv++;
    srand (time(NULL));
    
    int n = 10;
    if ( argc > 1 ){
        sscanf(argv[1], "%d", &n);
    }
    
    char* graph_name = (char*)"test/graphs/level2.json";
    if ( argc > 0 ){
        graph_name = argv[0];
    }
    graph g(graph_name);

    int num_agents[] = {2,3,5,10,20,50,100};
    string names[]  = {
        "         A*",
        "        DFS",
        "    A*mbush",
        "  P-A*mbush",
        "SAR-A*mbush"
    };
    
    int cases = 7;

    printf("Algorithm: Ambush-Rate Path-Increment(%%) Graph-Coverage(%%) Time(ms)\n");
    
    int num_tests = 100;
    for ( int c=0; c<cases; c++){
        vector< vector<float> > results;
        results.resize(5);
        for ( int i=0; i<5; i++){
            for (int i=0; i<4; i++){
                results[i].push_back(0.0);
            }
        }
        
        cout << "Num agents " <<  num_agents[c] << endl;
        
        for ( int i=0; i<num_tests; i++ ){
            world w(&g);
            noop np(&w);
            randomized_dfs dfs(&w);
            a_star ast(&w);
            ambush amb(&w);
            priority_ambush pamb(&w);
            self_adaptive_r_ambush sar(&w);
            
            agent target(n, &g, &w, &np, rand() % g.num_vertex());
            
            vector<agent*> agents;
            for ( int i=0; i<num_agents[c]; i++ ){
                agent* a = new agent(i, &g, &w, &np, rand() % g.num_vertex());
                agents.push_back(a);
                agents[i]->set_target(&target);
                w.add_agent(agents[i]);
            }
            
            eval_behavior(w, agents, target, &ast, results[0]);
            eval_behavior(w, agents, target, &dfs, results[1]);
            eval_behavior(w, agents, target, &amb, results[2]);
            eval_behavior(w, agents, target, &pamb, results[3]);
            //eval_behavior(w, agents, target, &sar, results[4]);
        }
        
        for ( int i=0; i<results.size(); i++ ){
            cout << names[i];
            for ( int j=0; j<results[i].size(); j++ ){
                printf(" %3.2f", results[i][j]/num_tests);
            }
            printf("\n");
        }
    }
    
    return 0;
}
