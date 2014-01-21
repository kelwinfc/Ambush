#include "behaviour.hpp"
#include <iostream>

int behaviour::next_step(agent* a)
{
    vector<int>* p_path;
    
    if ( !a->has_path() ){
        vector<int> path;
        this->get_plan(a, path);
    }
    
    p_path = a->get_path();
    
    if ( p_path->empty() ){
        return a->get_current_vertex();
    }
    
    return p_path->at(0);
}

noop::noop(world* w)
{
    this->w = w;
}

void noop::get_plan(agent* a, vector<int>& path)
{
    path.clear();
}

randomized_dfs::randomized_dfs(world* w)
{
    this->w = 0;
}
        
void randomized_dfs::get_plan(agent* a, vector<int>& path)
{
    stack< astar_node > s;
    int target, num_vertex;
    vector<int> parent; /* The predecessor of each node v in the traversed path
                         * from the source to the target going through v
                         */
    graph* g = a->get_graph();
    num_vertex = g->num_vertex();
    
    parent.resize( num_vertex);
    for ( int i = 0; i < num_vertex; i++ ){
        parent[i]    = -1;
    }
    
    target = a->get_target()->get_current_vertex();
    
    astar_node initial_node;
    initial_node.v = a->get_current_vertex();
    initial_node.p = a->get_current_vertex();
    
    s.push(initial_node);
    
    while( !s.empty() ){
        astar_node next = s.top();
        s.pop();
        
        int v = next.v;
        
        if ( parent[v] != -1 ){
            continue;
        }
        
        parent[v] = next.p;
        
        if ( v == target ){
            break;
        }
        
        /* Explore the neighbors of the current node */
        vector< pair<int, float> >* suc_aux = g->get_successors(v);
        vector< pair<int, float> > suc = *suc_aux;
        random_shuffle( suc.begin(), suc.end());
        
        for ( uint i = 0; i < suc.size(); i++ ){
            int w = suc[i].first;
            
            if ( parent[w] == -1 ){
                astar_node neighbor_node;
                neighbor_node.v = w;
                neighbor_node.p = v;
                s.push( neighbor_node );
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

increment_a_star::increment_a_star(){
    this->h = 0;
    this->I = 0;
}

increment_a_star::increment_a_star(void (*incr_f)(world*, agent*,
                                                  vector<float>&),
                                   world* w, heuristic* h)
{
    this->w = w;
    this->h = h;
    this->I = incr_f;
    
    if ( !h ){
        this->h = new h_zero(w);
        this->delete_h = true;
    }
}

increment_a_star::~increment_a_star(){
    if ( this->delete_h && this->h != 0){
        delete this->h;
        this->h = 0;
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
    
    if ( !this->h ){
        this->h = new h_zero(w);
        this->delete_h = true;
    }
    
    for ( int i = 0; i < num_vertex; i++ ){
        increment[i] = 1.0; /* It is assumed by the increment functions that
                             * this vector is initialized with ones.
                             */
        min_dist[i]  = -1.0;
        parent[i]    = -1;
    }
    
    if ( this->I != 0 ){
        this->I(w, a, increment);
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
        
        if ( a->get_current_vertex() != target )
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
        this->delete_h = true;
    }
}

void a_star::get_plan(agent* a, vector<int>& path)
{
    this->I = 0;
    increment_a_star::get_plan(a, path);
}

ambush::ambush(world* w, heuristic* h)
{
    this->w = w;
    this->h = h;
    
    if ( !h ){
        this->h = new h_zero(w);
        this->delete_h = true;
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
    
    a->clear_path();
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
    this->I = &ambush_increment_function;
    increment_a_star::get_plan(a, path);
}

bool index_priority(agent* a, agent* b){
    return a->get_index() < b->get_index();
}

priority_ambush::priority_ambush(world* w,
                                 bool (*priority_function)(agent*, agent*),
                                 heuristic* h)
{
    this->w = w;
    this->P = priority_function;
    this->h = h;
    
    if ( !h ){
        this->h = new h_zero(w);
        this->delete_h = true;
    }
}

void priority_ambush::get_plan(agent* a, vector<int>& path)
{
    this->I = &ambush_increment_function;
    
    vector<agent*>* agents_aux = this->w->get_agents();
    vector<agent*>::iterator it;
    vector<agent*> agents;
    for ( it = agents_aux->begin(); it != agents_aux->end(); ++it ){
        if ( !a->has_path() ){
            agents.push_back(*it);
        }
    }
    sort(agents.begin(), agents.end(), this->P);
    
    for ( it = agents.begin(); it != agents.end(); ++it ){
        agent* a = *it;
        if ( !a->has_path() ){
            vector<int> p;
            increment_a_star::get_plan(a, p);
            a->set_path(p);
        }
    }
    path = *a->get_path();
}

self_adaptive_r_ambush::self_adaptive_r_ambush()
{
    this->w               = 0;
    this->selector        = new noop_node_selector();
    this->h               = 0;
    this->delete_selector = true;
}

self_adaptive_r_ambush::self_adaptive_r_ambush(world* w, node_selector* ns,
                                               heuristic* h )
{
    this->w        = w;
    this->selector = ns;
    this->h        = h;
    
    if ( !h ){
        this->h = new h_zero(w);
        this->delete_h = true;
    }
    
    if ( !ns ){
        this->selector = new noop_node_selector(w);
        this->delete_selector = true;
    } else {
        this->delete_selector = false;
    }
}

self_adaptive_r_ambush::~self_adaptive_r_ambush()
{
    if ( this->delete_selector && this->selector != 0 ){
        delete this->selector;
        this->selector = 0;
    }
}

void self_adaptive_r_ambush::get_plan(agent* a, vector<int>& path)
{
    vector<int> min_path;
    vector<bool> chosen_nodes;
    ambush amb(this->w, this->h);
    a_star astar(this->w, this->h);
    set<int> nodes_in_path;
    int tmp_current_vertex = a->get_current_vertex();
    int target = a->get_target()->get_current_vertex();
    
    int increase_ambush = false;
    float best_uniformity = 0.0;
    vector<int> best_path;
    
    astar.get_plan(a, min_path);
    this->selector->select(a->get_graph(), min_path, chosen_nodes);
    this->get_pred_in_paths(nodes_in_path, target);
    
    a->set_path(min_path);
    best_uniformity = this->uniformity_metric(target);
    best_path = min_path;
    a->clear_path();
    int n=(int)min_path.size();
    for ( int i=n-1; i>=0; i-- ){
        if ( chosen_nodes[i] ){
            
            vector<int> ambush_path;
            vector<int> path;
            
            a->set_current_vertex(min_path[i]);
            amb.get_plan(a, ambush_path);
            
            concatenate_paths(min_path, ambush_path, i, path);
            int new_previous = target;
            if ( path.size() > 1 ){
                new_previous = path[ path.size() - 2 ];
            }
            
            a->set_path(path);
            float uniformity = this->uniformity_metric(target);
            a->clear_path();
            
            if ( !increase_ambush && 
                 nodes_in_path.find(new_previous) == nodes_in_path.end() )
            {
                /* The ambush rate is increased and therefore this is first
                 * point that reachs a better ambush
                 */
                best_path = path;
                best_uniformity = uniformity;
                increase_ambush = true;
            } else if ( uniformity > best_uniformity ){
                
                /* If the Ambush rate is not increased, select the cutting
                 * point that generates the best distribution of the agents
                 */
                best_path = path;
                best_uniformity = uniformity;
            }
            
            a->set_current_vertex(tmp_current_vertex);
        }
    }
    path = best_path;
}

void self_adaptive_r_ambush::get_pred_in_paths(set<int>& nodes, int target)
{
    nodes.clear();
    
    vector<agent*>* agents = this->w->get_agents();
    vector<agent*>::iterator it_agents;
    for ( it_agents = agents->begin(); it_agents != agents->end(); ++it_agents )
    {
        agent* a = *it_agents;
        if ( a->has_path() ){
            vector<int>* path = a->get_path();
            if ( path->size() > 1 && path->back() == target ){
                int prev = path->at( path->size() - 2 );
                nodes.insert(prev);
            }
        }
    }
}

float self_adaptive_r_ambush::uniformity_metric(int target)
{
    float ret = 0.0;
    
    uint n = 0;
    map<int, int> num;
    
    graph* g = this->w->get_graph();
    vector< pair<int, float> >* pred_t = g->get_predecessors(target);
    vector< pair<int, float> >::iterator it_pred;
    float num_pred_t = (float)pred_t->size();
    
    vector<agent*>* agents = this->w->get_agents();
    vector<agent*>::iterator it_agents;
    for ( it_agents = agents->begin(); it_agents != agents->end(); ++it_agents )
    {
        agent* a = *it_agents;
        if ( a->has_path() ){
            
            vector<int>* path = a->get_path();
            if ( path->size() > 1 && path->back() == target ){
                int prev = path->at( path->size() - 2 );
                if ( num.find(prev) == num.end() ){
                    num[prev] = 1;
                } else {
                    num[prev]++;
                }
            }
            
            n++;
        }
    }
    
    for ( it_pred = pred_t->begin(); it_pred != pred_t->end(); ++it_pred ){
        int i = it_pred->first;
        
        int diff = num[i] - (int)ceil((float)n / num_pred_t);
        
        if ( diff > 0 ){
            ret += diff;
        }
    }
    
    return 1.0 - ret / ( n - (int)floor((float)n / num_pred_t )+ 1e-6);
}

r_ambush::r_ambush(world* w, float r, heuristic* h)
{
    this->w = w;
    this->h = h;
    this->r = r;
    
    if ( !h ){
        this->h = new h_zero(w);
        this->delete_h = true;
    }
    
    this->selector = new r_node_selector(this->w, this->r);
    this->delete_selector = true;
}

void r_ambush::get_plan(agent* a, vector<int>& path)
{
    self_adaptive_r_ambush::get_plan(a, path);
}
