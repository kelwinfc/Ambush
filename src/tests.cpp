#include "tests.hpp"

void eval_behavior(world& w, vector<agent*>& agents, agent& target,
                   behavior* b, vector<float>& results)
{
    int n = (int)agents.size();
    
    for ( int i=0; i<n; i++ ){
        agents[i]->set_behavior(b);
    }
    w.clear_paths();
    
    //const clock_t begin_time = clock();
    w.compute_paths(&target);
    //const clock_t end_time = clock();

    results[0] += w.ambush_rate_relaxed(&target);
    results[1] += w.ambush_rate(&target);
    results[2] += w.increment_rate(&target);
    results[3] += w.graph_coverage();
}

void vvector_to_pred_set(vector<edge>& v, set<int>& s)
{
    vector<edge>::iterator it;
    for ( it = v.begin(); it != v.end(); it++ ){
        s.insert(it->from);
    }
}

int get_initial_position(int target, graph& g)
{
    int ret = rand() % g.num_vertex();
    vector<edge>* vpred = g.get_predecessors(target);
    set<int> pred;
    vvector_to_pred_set(*vpred, pred);

    while ( ret == target || pred.find(ret) != pred.end() )
    {
        ret = rand() % g.num_vertex();
    }
    
    return ret;
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

    int num_agents[] = {2, 4, 8, 16};
    int cases = 4;
    
    string names[]  = {
        "         A*",
        /*"        DFS",*/
        "    A*mbush",
        "  P-A*mbush",
        "SAR-A*mbush"
    };
    
    int num_tests = 100;
    int initial_position = 0;
    
    int num_algorithms = 4;
    int num_metrics = 4;
    
    vector< vector< vector<float> > > results;

    for ( int c = 0; c < cases; c++ ){

        vector< vector<float> > next_case;
        
        for ( int i = 0; i < num_algorithms; i++){
            
            vector<float> next;
            
            for (int i = 0; i < num_metrics; i++){
                next.push_back(0.0);
            }
            
            next_case.push_back(next);
        }

        results.push_back(next_case);
    }
    
    for ( int i = 0; i < num_tests; i++ ){
        world w(&g);
        noop np(&w);
        a_star ast(&w);
        ambush amb(&w);
        priority_ambush pamb(&w);
        self_adaptive_r_ambush sar(&w);
        density_crowd dc(&w);

        cout << "Test " << i+1 << "/" << num_tests << endl;
        
        int target_vertex = rand() % g.num_vertex();
        int initial_position = get_initial_position(target_vertex, g);
        vector<agent*> agents;
        
        for (int c = 0; c < cases; c++ ){
            agent target(num_agents[c]+1, &g, &w, &np, target_vertex);
            
            // Add the additional agents
            while ( (int)agents.size() < num_agents[c] ){
                
                if ( !use_same_position ){
                    initial_position = get_initial_position(target_vertex, g);
                }
                
                agent* a = new agent(i, &g, &w, &np, initial_position);
                agents.push_back(a);
                agents.back()->set_target(&target);
                w.add_agent(agents.back());
            }
            
            // Clear agents' path and set the current target
            for ( size_t a = 0; a < agents.size(); a++ ){
                agents[a]->set_target(&target);
                agents[a]->clear_path();
            }
            
            // Eval
            eval_behavior(w, agents, target, &ast, results[c][0]);
            eval_behavior(w, agents, target, &amb, results[c][1]);
            eval_behavior(w, agents, target, &pamb, results[c][2]);
            eval_behavior(w, agents, target, &sar, results[c][3]);
        }
        
        for ( size_t a = 0; a < agents.size(); a++ ){
            delete agents[a];
        }
        
        cout << "Test " << i+1 << "/" << num_tests << "... done" << endl;
        
        for (int c = 0; c < cases; c++ ){
            cout << " & " << num_agents[c];
            for ( int a = 0; a < num_algorithms; a++ ){
                for ( int j = 0; j < 2; j++ ){
                    printf(" & %3.2f", results[c][a][j] / (i+1));
                }
            }
            printf("\\\\\n");
        }
    }
    
    return 0;
}
