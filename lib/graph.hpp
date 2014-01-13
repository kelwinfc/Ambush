#ifndef __GRAPH
#define __GRAPH

#include <vector>
#include <string>
#include <cassert>

#include "utils.hpp"

using namespace std;

class graph {
    
    private:
        vector< vector< pair<int, float> > > suc;
        vector< vector< pair<int, float> > > pred;
        bool is_directed;
    
    public:
        
        graph(bool directed=false);
        
        //TODO define a json format to describe a graph
        graph(string& filename);
        
        int add_vertex();
        
        void add_edge( int v, int w, float cost );
        
        int num_vertex();
        
        vector< pair<int, float> >* get_sucessors( int v );
        vector< pair<int, float> >* get_predecessors( int w );
};

#endif