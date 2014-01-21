#ifndef __AMBUSH_WORLD
#define __AMBUSH_WORLD

#include <vector>
#include <set>

#include "behavior.hpp"
#include "agent.hpp"
#include "utils.hpp"

using namespace std;

class behaviour;
class agent;

class world {
    private:
        vector<agent*> agents;
        graph* g;
        
    public:
        world(graph* g);
        
        //TODO define a json format to describe a problem instance
        world(string& filename);
        
        void add_agent(agent* a);
        void clear_paths();
        int num_agents();
        vector<agent*>* get_agents();
        
        void compute_paths(agent* target);
        float ambush_rate(agent* target);
        float increment_rate(agent* target);
        
        graph* get_graph();
};

#endif