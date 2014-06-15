#include "diff_ambush_density.hpp"

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
    
    results[0] += w.ambush_rate(&target);
    results[1] += w.increment_rate(&target);
    results[2] += w.graph_coverage();
}

int main(int argc, char* argv[])
{
    argc--;
    argv++;
    srand (time(NULL));
    
    char* graph_name = (char*)"test/graphs/level2.json";
    if ( argc > 0 ){
        graph_name = argv[0];
    }
    graph g(graph_name);
    
    int num_tests = 100;
    int initial_position = rand() % g.num_vertex();
    
    int num_algorithms = 3;
    int num_pow_agents = 12;
    
    for ( int i=11; i<num_pow_agents; i++ ){
        int num_agents = 1 << i;
        
        vector< vector<float> > results;
        for ( int i=0; i<num_algorithms; i++){
            vector<float> next;
            for (int i=0; i<3; i++){
                next.push_back(0.0);
            }
            results.push_back(next);
        }
        
        for ( int t=0; t<num_tests; t++ ){
            world w(&g);
            noop np(&w);
            a_star ast(&w);
            ambush amb(&w);
            density_crowd dc(&w);
            
            agent target(num_agents, &g, &w, &np, rand() % g.num_vertex());
            
            vector<agent*> agents;
            for ( int i=0; i<num_agents; i++ ){
                agent* a = new agent(i, &g, &w, &np, initial_position);
                agents.push_back(a);
                agents[i]->set_target(&target);
                w.add_agent(agents[i]);
            }
            
            eval_behavior(w, agents, target, &ast, results[0]);
            //eval_behavior(w, agents, target, &dfs, results[1]);
            eval_behavior(w, agents, target, &dc, results[1]);
            eval_behavior(w, agents, target, &amb, results[2]);
            
            cout << t+1 << " " << num_agents << " " << i;
            for ( int k=0; k<results.size(); k++ ){
                //cout << names[i];
                for ( int j=0; j<3; j++ ){
                    printf(" %3.2f", results[k][j]/(t+1));
                }
            }
            printf("\n");
        }
        
    }
    
    return 0;
}
