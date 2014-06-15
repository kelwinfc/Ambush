#include "graph.hpp"
#include <iostream>

node::node()
{
}

node::node(const args_manager& args){
    this->args = args;
}

edge::edge(int from, int to, float cost)
{
    this->from = from;
    this->to   = to;
    this->cost = cost;
}

edge::edge(int from, int to, float cost, const args_manager& args)
{
    this->from = from;
    this->to   = to;
    this->cost = cost;
    this->args = args;
}


bool operator<(const edge& a, const edge& b)
{
    if ( a.cost != b.cost ){
        return a.cost < b.cost;
    }
    return a.from < b.from || ( a.from == b.from && a.to < b.to );
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
    
    nodes.resize(this->num_vertex());
    
    const rapidjson::Value& nodes_descr = document["nodes"];
    if(nodes_descr.IsArray()){
        for (rapidjson::SizeType i = 0; i < nodes_descr.Size(); i++){
            
            const rapidjson::Value& next_node = nodes_descr[i];
            
            args_manager node_args(next_node);
            nodes[i] = node(node_args);
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
            const rapidjson::Value& args_json = next_edge["args"];
            
            if ( src_json.IsInt() && dst_json.IsInt() && cost_json.IsNumber() )
            {
                int src, dst;
                float cost;
                
                src  = src_json.GetInt();
                dst  = dst_json.GetInt();
                cost = (float)cost_json.GetDouble();
                args_manager args(args_json);
                
                if ( 0 <= src && src < (int)this->suc.size() &&
                     0 <= dst && dst < (int)this->suc.size() && 0 <= cost )
                {
                    edge e(src, dst, cost, args);
                    this->add_edge(e);
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

void graph::add_edge( const edge& e )
{
    edge e_back(e.to, e.from, e.cost, e.args);
    
    this->suc[e.from].push_back(e);
    this->pred[e.to].push_back(e);
    
    if ( !this->is_directed ){
        this->suc[e_back.from].push_back(e_back);
        this->pred[e_back.to].push_back(e_back);
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

node* graph::get_node(int v)
{
    return &this->nodes[v];
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

void vector_to_pred_set(vector<edge>& v, set<int>& s)
{
    vector<edge>::iterator it;
    for ( it = v.begin(); it != v.end(); it++ ){
        s.insert(it->from);
    }
}

void graph::get_reachable_predecessors(int source, int target,
                                       set<int>& reachable_predecessors)
{
    queue<int> q;
    vector<bool> visited;
    set<int> predecessors;

    vector_to_pred_set(this->pred[target], predecessors);
    
    reachable_predecessors.clear();
    visited.resize(this->num_vertex());
    fill(visited.begin(), visited.end(), 0);
    
    q.push(source);
    while ( !q.empty() ){
        int v = q.front();
        q.pop();

        if ( visited[v] || v == target ){
            continue;
        }
        
        if ( predecessors.find(v) != predecessors.end() ){
            reachable_predecessors.insert(v);
        }
        
        visited[v] = true;
        
        uint num_suc = this->suc[v].size();
        for ( uint i = 0; i < num_suc; i++ ){
            int w = this->suc[v][i].to;
            if ( !visited[w] && w != target ){
                q.push(w);
            }
        }
    }
}

int graph::get_index(int from, int to)
{
    uint num_suc = this->suc[from].size();
    for ( uint i = 0; i < num_suc; i++ ){
        if ( this->suc[from][i].to == to ){
            return i;
        }
    }
    return -1;
}

float graph::mbm_find_path()
{
    queue<int> q;
    vector<bool> visited;
    vector<int> parent;
    
    q.push(0);

    visited.resize( this->num_vertex() );
    parent.resize( this->num_vertex() );
    
    fill(visited.begin(), visited.end(), 0);
    fill(parent.begin(), parent.end(), -1);
    
    visited[0] = true;

    bool keep = true;
    
    while ( !q.empty() && keep ){
        int v = q.front();
        q.pop();

        uint num_suc = this->suc[v].size();
        for ( uint i = 0; i < num_suc && keep; i++ ){
            int w = this->suc[v][i].to;

            if ( !visited[w] && this->suc[v][i].cost > 0 ){
                q.push(w);
                visited[w] = true;
                parent[w] = v;
                
                if ( w == 1 ){
                    keep = false;
                    break;
                }
            }
        }
    }
    
    int v = 1;
    float path_cap = -1.0;
    
    while ( parent[v] > -1 ){
        int prev = parent[v];
        int i_prev_v = this->get_index(prev, v);
        if ( path_cap < 0 ||
                this->suc[prev][i_prev_v].cost < path_cap ){
            path_cap = this->suc[prev][i_prev_v].cost;
        }
        v = prev;
    }
    
    v = 1;
    while ( parent[v] != -1 ){
        int prev = parent[v];
        int i_prev_v = this->get_index(prev, v);
        int i_v_prev = this->get_index(v, prev);
        
        this->suc[prev][i_prev_v].cost -= path_cap;
        this->suc[v][i_v_prev].cost += path_cap;
        v = prev;
    }

    if ( path_cap < 0 ){
        return 0.0;
    }
    
    return path_cap;
}

float graph::maximum_bipartite_matching(vector< pair<int, int> >& match)
{
    float ret = 0.0;
    
    while ( true ){
        float path_capacity = this->mbm_find_path();
        if ( path_capacity < 1e-6 ){
            break;
        }
        
        ret += path_capacity;
    }
    
    int n = this->num_vertex();
    
    for ( int i = 2; i < n; i++ ){
        int m = this->suc[i].size();
        for ( int j = 0; j < m; j++ ){
            if ( this->suc[i][j].to == 1 && this->suc[i][j].cost < 1.0 ){
                match.push_back( make_pair(i,j) );
            }
        }
    }
    return ret;
}
