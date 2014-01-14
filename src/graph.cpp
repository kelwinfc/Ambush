#include "graph.hpp"
#include <iostream>

graph::graph(bool directed)
{
    this->is_directed = directed;
}

graph::graph(char* filename)
{
    FILE * pFile = fopen (filename, "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document document;
    document.ParseStream<0>(is);
    
    if ( document["name"].IsString() ){
        this->name = document["name"].GetString();
    }
    
    if ( document["directed"].IsBool() ){
        this->is_directed = document["directed"].GetBool();
    }
    
    int n = 0;
    if ( document["num_nodes"].IsInt() ){
        n = document["num_nodes"].GetInt();
    }
    
    while( n-- ){
        this->add_vertex();
    }
    
    const rapidjson::Value& edges = document["edges"];
    if(edges.IsArray()){
        for (rapidjson::SizeType i = 0; i < edges.Size(); i++){
            
            const rapidjson::Value& next_edge = edges[i];
            
            if ( !next_edge.IsObject() ){
                continue;
            }
            
            const rapidjson::Value& src_json  = next_edge["src"];
            const rapidjson::Value& dst_json  = next_edge["dst"];
            const rapidjson::Value& cost_json = next_edge["cost"];
            
            if ( src_json.IsInt() && dst_json.IsInt() && cost_json.IsNumber() )
            {
                int src, dst;
                float cost;
                
                src  = src_json.GetInt();
                dst  = dst_json.GetInt();
                cost = (float)cost_json.GetDouble();
                
                if ( 0 <= src && src < (int)this->suc.size() &&
                     0 <= dst && dst < (int)this->suc.size() && 0 <= cost )
                {
                    this->add_edge(src, dst, cost);
                }
            }
        }
    }
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
