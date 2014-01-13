#ifndef __AMBUSH_GRAPH
#define __AMBUSH_GRAPH

#include <vector>
#include <string>
#include <cassert>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"

#include "utils.hpp"

using namespace std;

class graph {
    
    private:
        string name;
        vector< vector< pair<int, float> > > suc;
        vector< vector< pair<int, float> > > pred;
        bool is_directed;
    
    public:
        graph(bool directed=false);
        
        graph(char* filename);
        
        int add_vertex();
        
        void add_edge( int v, int w, float cost );
        
        int num_vertex();
        
        vector< pair<int, float> >* get_successors( int v );
        vector< pair<int, float> >* get_predecessors( int w );
};

#endif