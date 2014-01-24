#ifndef __AMBUSH_HEURISTIC
#define __AMBUSH_HEURISTIC

#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"
#include "world.hpp"
#include "args_manager.hpp"

using namespace std;

class agent;
class world;

class heuristic {
    protected:
        world* w;
        graph* g;
    
    public:
        heuristic();
        heuristic(world* w);
        
        virtual float h(int v, int t);
};

class h_zero : public heuristic {
    public:
        h_zero(world* w);
        
        virtual float h(int v, int t);
};

class h_euclidean : public heuristic {
    public:
        h_euclidean(world* w);
        
        virtual float h(int v, int t);
};

#endif