#include "node_selector.hpp"

node_selector::node_selector()
{
    this->w = 0;
}

node_selector::node_selector(world* w)
{
    this->w = w;
}

void node_selector::select(graph* g, vector<int>& path,
                           vector<bool>& selected)
{
    selected.resize(path.size());
    fill(selected.begin(), selected.end(), false);
}

noop_node_selector::noop_node_selector()
{
    this->w = 0;
}

noop_node_selector::noop_node_selector(world* w)
{
    this->w = w;
}

void noop_node_selector::select(graph* g, vector<int>& path,
                                vector<bool>& selected)
{
    selected.resize(path.size());
    fill(selected.begin(), selected.end(), true);
}

last_node_selector::last_node_selector()
{
    this->w = 0;
}

last_node_selector::last_node_selector(world* w)
{
    this->w = w;
}

void last_node_selector::select(graph* g, vector<int>& path,
                                vector<bool>& selected)
{
    if ( path.empty() )
        return;
    
    selected.resize(path.size());
    fill(selected.begin(), selected.end(), false);
    selected[ path.size() - 1 ] = true;
}

r_node_selector::r_node_selector()
{
    this->w = 0;
    this->r = 0.0;
}

r_node_selector::r_node_selector(world* w, float r)
{
    this->w = w;
    this->r = r;
}

void r_node_selector::select(graph* g, vector<int>& path,
                             vector<bool>& selected)
{
    float ac = 0.0;
    int i = (int)path.size()-1;
    
    selected.resize(path.size());
    fill(selected.begin(), selected.end(), false);
    
    for ( ; i > 0 && ac < r; i-- ){
        ac += g->edge_cost(path[i], path[i-1]);
    }
    
    if ( 0 <= i && i < (int)path.size() ){
        selected[i] = true;
    }
}
