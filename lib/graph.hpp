#ifndef __AMBUSH_GRAPH
#define __AMBUSH_GRAPH

#include <vector>
#include <map>
#include <string>
#include <cassert>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"

#include "utils.hpp"
#include "args_manager.hpp"

using namespace std;

class edge {
    
    public:
        int from;
        int to;
        float cost;
        args_manager args;
        
        edge(int from, int to, float cost);
        edge(int from, int to, float cost, args_manager& args);
};

class graph {
    
    private:
        string name;
        vector< vector< edge > > suc;
        vector< vector< edge > > pred;
        
        bool is_directed;
        vector< args_manager > args;
    
    public:
        graph(bool directed=false);
        
        graph(char* filename);
        
        int add_vertex();
        
        void add_edge( int v, int w, float cost );
        
        int num_vertex();
        
        vector< edge >* get_successors( int v );
        vector< edge >* get_predecessors( int w );
        
        args_manager* get_args(int v);
        
        float edge_cost(int v, int w);
        float path_cost(vector<int>& path);
};

#endif