#include "behaviour.hpp"
#include <iostream>

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

increment_a_star::increment_a_star(void (*incr_f)(world*, agent*,
                                                  vector<float>&),
                                   world* w, heuristic* h)
{
    this->w = w;
    this->h = h;
    this->increment_function = incr_f;
    
    if ( !h ){
        this->h = new h_zero(w);
    }
}

void increment_a_star::get_plan(agent* a, vector<int>& path)
{
    priority_queue< astar_node > q;
    int target, num_vertex;
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
    num_vertex = g->num_vertex();
    
    increment.resize( num_vertex );
    min_dist.resize( num_vertex );
    parent.resize( num_vertex);
    
    for ( int i = 0; i < num_vertex; i++ ){
        increment[i] = 1.0; /* It is assumed by the increment functions that
                             * this vector is initialized with ones.
                             */
        min_dist[i]  = -1.0;
        parent[i]    = -1;
    }
    
    if ( this->increment_function != 0 ){
        this->increment_function(w, a, increment);
    }
    /* Initialize the visited nodes */
    target = a->get_target()->get_current_vertex();
    
    astar_node initial_node;
    initial_node.v = a->get_current_vertex();
    initial_node.p = a->get_current_vertex();
    initial_node.f = initial_node.g = 0.0;
    
    q.push(initial_node);
    min_dist[ a->get_current_vertex() ] = 0.0;
    
    while( !q.empty() ){
        astar_node next = q.top();
        q.pop();
        
        int d = next.g;
        int v = next.v;
        /* If the current expansion is worse than the best found */
        if ( min_dist[v] != -1 && min_dist[v] < d ){
            continue;
        }
        
        /* Update the information of the current node */
        min_dist[v] = d;
        parent[v] = next.p;
        
        if ( v == target ){
            break;
        }
        
        /* Explore the neighbors of the current node */
        vector< pair<int, float> >* suc = g->get_successors(v);
        
        for ( uint i = 0; i < suc->size(); i++ ){
            int w = suc->at(i).first;
            /* The new distance involves the distance from the source to the
             * current vertex (v), the cost from v to its neighbor (w) and
             * the increment function of the node successor
             */
            int nd = d + suc->at(i).second * increment[w] * increment[w];
            if ( min_dist[w] == -1 || nd < min_dist[w] ){
                astar_node neighbor_node;
                neighbor_node.v = w;
                neighbor_node.p = v;
                neighbor_node.f = nd + this->h->h(w, target);
                neighbor_node.g = nd;
                q.push( neighbor_node );
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
        path.push_back(v);
        
        reverse( path.begin(), path.end() );
    }
}

a_star::a_star(world* w, heuristic* h)
{
    this->w = w;
    this->h = h;
    
    if ( !h ){
        this->h = new h_zero(w);
    }
}

void a_star::get_plan(agent* a, vector<int>& path)
{
    this->increment_function = 0;
    increment_a_star::get_plan(a, path);
}

ambush::ambush(world* w, heuristic* h)
{
    this->w = w;
    this->h = h;
    
    if ( !h ){
        this->h = new h_zero(w);
    }
}

/* Compute the Increment function of each vertex in the graph
 * This is a non-amortized version of A*mbush. The increment function could
 * be precomputed.
 */
void ambush_increment_function(world* w, agent* a, vector<float>& increment)
{
    vector<agent*>* agents = w->get_agents();
    int num_agents = agents->size();
    vector<int> path;
    
    a->set_path(path);
    
    for ( int i = 0; i < num_agents; i++ ){
        vector<int>* next_path = agents->at(i)->get_path();
        vector<int>::iterator it;
        
        for ( it = next_path->begin(); it != next_path->end(); ++it ){
            increment[*it] += 1.0;
        }
    }
}

void ambush::get_plan(agent* a, vector<int>& path)
{
    this->increment_function = &ambush_increment_function;
    increment_a_star::get_plan(a, path);
}
