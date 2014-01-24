#include "graph.hpp"
#include <iostream>

edge::edge(int from, int to, float cost)
{
    this->from = from;
    this->to   = to;
    this->cost = cost;
}

edge::edge(int from, int to, float cost, args_manager& args)
{
    this->from = from;
    this->to   = to;
    this->cost = cost;
    this->args = args;
}

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
    
    args.resize(this->num_vertex());
    
    const rapidjson::Value& nodes = document["nodes"];
    if(nodes.IsArray()){
        for (rapidjson::SizeType i = 0; i < nodes.Size(); i++){
            
            const rapidjson::Value& next_node = nodes[i];
            
            args[i] = args_manager(next_node);
        }
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
    vector< edge > s, p;
    this->suc.push_back(s);
    this->pred.push_back(p);
    
    return this->num_vertex();
}

void graph::add_edge( int v, int w, float cost )
{
    edge e(v, w, cost);
    edge e_back(w, v, cost);
    
    this->suc[v].push_back(e);
    this->pred[w].push_back(e);
    
    if ( !this->is_directed ){
        this->suc[w].push_back(e_back);
        this->pred[v].push_back(e_back);
    }
}

int graph::num_vertex(){
    return this->suc.size();
}

vector< edge >* graph::get_successors( int v )
{
    assert( 0 <= v && (unsigned int)v < this->suc.size() );
    return &this->suc[v];
}

vector< edge >* graph::get_predecessors( int w )
{
    assert( 0 <= w && (unsigned int)w < this->pred.size() );
    return &this->pred[w];
}

args_manager* graph::get_args(int v)
{
    return &this->args[v];
}

float graph::edge_cost(int v, int w)
{
    for ( uint j=0; j<suc[v].size(); j++ ){
        int next = suc[v][j].to;
        if ( next == w ){
            return suc[v][j].cost;
        }
    }
    return 0.0;
}

float graph::path_cost(vector<int>& path)
{
    float ret = 0.0;
    uint n = path.size();
    for ( uint i=1; i<n; i++ ){
        ret += this->edge_cost(path[i-1], path[i]);
    }
    
    return ret;
}
