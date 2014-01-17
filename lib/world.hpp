#ifndef __AMBUSH_WORLD
#define __AMBUSH_WORLD

#include <vector>
#include <set>

#include "behaviour.hpp"
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
        float ambush_rate(agent* target);
        void clear_paths();
        void compute_paths(agent* target);
        int num_agents();
        
        vector<agent*>* get_agents();
};

#endif