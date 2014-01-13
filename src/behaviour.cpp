#include "behaviour.hpp"

int behaviour::next_step(agent* a)
{
    vector<int> path;
    this->get_plan(a, path);
    
    if ( path.empty() ){
        return a->get_current_vertex();
    }
    
    return path[0];
}

noop::noop(world* w)
{
    this->w = w;
}

void noop::get_plan(agent* a, vector<int>& path)
{
    path.clear();
}

ambush::ambush(world* w)
{
    this->w = w;
}

void ambush::get_plan(agent* a, vector<int>& path)
{
    priority_queue< pair<float, pair<int, int> > > q;
    int target, num_vertex, num_agents;
    vector<float> increment;    // Increment cost associated to each vertex
    vector<float> min_dist;     /* Shortest path from the current position of
                                 * the agent to the target through a given
                                 * vertex
                                 */
    vector<int> parent;         /* The predecessor of each node v in a shortest
                                 * path from the source to the target going
                                 * through v
                                 */
    graph* g = a->get_graph();
    
    num_agents = this->w->num_agents();
    num_vertex = g->num_vertex();
    
    
    increment.resize( num_vertex );
    min_dist.resize( num_vertex );
    
    for ( int i = 0; i < num_vertex; i++ ){
        increment[i] = 1.0;
        min_dist[i]  = -1.0;
        parent[i]    = -1;
    }
    
    /* Compute the Increment function of each vertex in the graph
     * This is a non-amortized version of A*mbush. The increment function could
     * be precomputed.
     */
    {
        vector<agent*>* agents = this->w->get_agents();
        path.clear();
        a->set_path(path);
        
        for ( int i = 0; i < num_agents; i++ ){
            vector<int>* next_path = agents->at(i)->get_path();
            vector<int>::iterator it;
            
            for ( it = next_path->begin(); it != next_path->end(); ++it ){
                increment[*it] += 1.0;
            }
        }
    }
    
    /* Initialize the visited nodes */
    target = a->get_target()->get_current_vertex();
    q.push( make_pair(0.0,
                      make_pair(a->get_current_vertex(),
                                a->get_current_vertex())
                     )
          );
    min_dist[ a->get_current_vertex() ] = 0.0;
    
    while( !q.empty() ){
        pair<float, pair<int, int> > next = q.top();
        q.pop();
        
        int d = -next.first; /* C++ implementation of priority_queue is a
                              * max-heap so it is needed to store the opposite
                              * cost in order to maintain the right order
                              */
        int v = next.second.first;
        
        /* If the current expansion is worse than the best found */
        if ( min_dist[v] != -1 && min_dist[v] < d ){
            continue;
        }
        
        /* Update the information of the current node */
        min_dist[v] = d;
        parent[v] = next.second.second;
        
        if ( v == target ){
            break;
        }
        
        /* Explore the neighbors of the current node */
        vector< pair<int, float> >* suc = g->get_sucessors(v);
        for ( uint i = 0; i < suc->size(); i++ ){
            int w = suc->at(i).first;
            
            /* The new distance involves the distance from the source to the
             * current vertex (v), the cost from v to its neighbor (w) and
             * the increment function of the node successor
             */
            int nd = d + suc->at(i).second * increment[w] * increment[w];
            
            if ( min_dist[w] == -1 || nd < min_dist[w] ){
                q.push( make_pair( -nd, make_pair(w, v) ) );
                min_dist[w] = nd;
            }
        }
    }
    
    if ( parent[target] != -1 ){
        /* Reconstruct the path. Note that it is recovered backwards so it is
         * needed to reverse it at the end.
         */
        int v = target;
        do {
            path.push_back(v);
            v = parent[v];
        } while ( v != parent[v] );
        
        reverse( path.begin(), path.end() );
    }
}
