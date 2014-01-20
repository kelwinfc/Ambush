#ifndef __AMBUSH_NODE_SELECTOR
#define __AMBUSH_NODE_SELECTOR

#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"
#include "world.hpp"

using namespace std;

class agent;
class world;
class graph;

class node_selector {
    protected:
        world* w;
    
    public:
        node_selector();
        node_selector(world* w);
        
        virtual void select(graph* g, vector<int>& path,
                            vector<bool>& selected);
};

class noop_node_selector : public node_selector {
    public:
        noop_node_selector();
        noop_node_selector(world* w);
        
        virtual void select(graph* g, vector<int>& path,
                            vector<bool>& selected);
};

class last_node_selector : public node_selector {
    public:
        last_node_selector();
        last_node_selector(world* w);
        
        virtual void select(graph* g, vector<int>& path,
                            vector<bool>& selected);
};

class r_node_selector : public node_selector {
    private:
        float r;
    
    public:
        r_node_selector();
        r_node_selector(world* w, float r);
        
        virtual void select(graph* g, vector<int>& path,
                            vector<bool>& selected);
};

#endif