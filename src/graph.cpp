#include "graph.hpp"

graph::graph(bool directed)
{
    this->is_directed = directed;
}

graph::graph(string& filename)
{
    //TODO
}

int graph::add_vertex()
{
    vector< pair<int, float> > s, p;
    this->suc.push_back(s);
    this->pred.push_back(p);
    
    return this->num_vertex();
}

void graph::add_edge( int v, int w, float cost )
{
    this->suc[v].push_back( make_pair(w, cost) );
    this->pred[w].push_back( make_pair(v, cost) );
    
    if ( !this->is_directed ){
        this->suc[w].push_back( make_pair(v, cost) );
        this->pred[v].push_back( make_pair(w, cost) );
    }
}

int graph::num_vertex(){
    return this->suc.size();
}

vector< pair<int, float> >* graph::get_successors( int v )
{
    assert( 0 <= v && (unsigned int)v < this->suc.size() );
    return &this->suc[v];
}

vector< pair<int, float> >* graph::get_predecessors( int w )
{
    assert( 0 <= w && (unsigned int)w < this->pred.size() );
    return &this->pred[w];
}
