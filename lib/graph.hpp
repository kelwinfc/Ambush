#ifndef __AMBUSH_GRAPH
#define __AMBUSH_GRAPH

#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"

#include "utils.hpp"
#include "args_manager.hpp"

using namespace std;

class node {
    
    public:
        args_manager args;
        
        node();
        node(const args_manager& args);
    
};

class edge {
    
    public:
        int from;
        int to;
        float cost;
        args_manager args;
        
        edge(int from, int to, float cost);
        edge(int from, int to, float cost, const args_manager& args);

        friend bool operator<(const edge &, const edge &);
};

class graph {
    
    private:
        string name;
        vector< vector< edge > > suc;
        vector< vector< edge > > pred;
        
        bool is_directed;
        vector< node > nodes;
    
    public:
        graph(bool directed=false);
        
        graph(char* filename);
        
        int add_vertex();
        
        void add_edge( const edge& e );
        
        int num_vertex();
        
        vector< edge >* get_successors( int v );
        vector< edge >* get_predecessors( int w );
        
        node* get_node(int v);
        
        float edge_cost(int v, int w);
        float path_cost(vector<int>& path);

        float maximum_bipartite_matching(vector< pair<int, int> >& match);
        
        void get_reachable_predecessors(int source, int target,
                                        set<int>& predecessors);

    private:
        float mbm_find_path();
        int get_index(int from, int to);
};

#endif