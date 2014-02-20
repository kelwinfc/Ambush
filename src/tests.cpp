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
    results[1] += w.ambush_rate(&target);
    results[2] += w.increment_rate(&target);
    results[3] += w.graph_coverage();
}

int main(int argc, char* argv[])
{
    argc--;
    argv++;
    srand (time(NULL));
    
    bool use_same_position = false;
    
    if ( argc > 1 && argv[1][0] == '1' ){
        use_same_position = true;
    }
    
    char* graph_name = (char*)"test/graphs/level2.json";
    if ( argc > 0 ){
        graph_name = argv[0];
    }
    graph g(graph_name);

    int num_agents[] = {2,/*3,*/5,10,20,/*50,100*/};
    int cases = 4;
    
    string names[]  = {
        "         A*",
        /*"        DFS",*/
        "    A*mbush",
        "  P-A*mbush",
        "SAR-A*mbush"
    };


    //printf("Algorithm: Ambush-Rate Path-Increment(%%) Graph-Coverage(%%) Time(ms)\n");
    
    int num_tests = 100;
    int initial_position = rand() % g.num_vertex();
    
    for ( int c=0; c<cases; c++){
        vector< vector<float> > results;
        for ( int i=0; i<4; i++){
            vector<float> next;
            for (int i=0; i<4; i++){
                next.push_back(0.0);
            }
            results.push_back(next);
        }
        
        cout << /*"Num agents " <<  */ " & " << num_agents[c];
        
        for ( int i=0; i<num_tests; i++ ){
            world w(&g);
            noop np(&w);
            randomized_dfs dfs(&w);
            a_star ast(&w);
            ambush amb(&w);
            priority_ambush pamb(&w);
            self_adaptive_r_ambush sar(&w);
            
            agent target(num_agents[c]+1, &g, &w, &np, rand() % g.num_vertex());
            
            vector<agent*> agents;
            for ( int i=0; i<num_agents[c]; i++ ){
                if ( !use_same_position ){
                    initial_position = rand() % g.num_vertex();
                }
                
                agent* a = new agent(i, &g, &w, &np, initial_position);
                agents.push_back(a);
                agents[i]->set_target(&target);
                w.add_agent(agents[i]);
            }
            
            eval_behavior(w, agents, target, &ast, results[0]);
            //eval_behavior(w, agents, target, &dfs, results[1]);
            eval_behavior(w, agents, target, &amb, results[1]);
            eval_behavior(w, agents, target, &pamb, results[2]);
            eval_behavior(w, agents, target, &sar, results[3]);
        }
        
        for ( int i=0; i<results.size(); i++ ){
            //cout << names[i];
            for ( int j=0; j<2; j++ ){
                printf(" & %3.2f", results[i][j]/num_tests);
            }
        }
        printf("\\\\\n");
    }
    
    return 0;
}
