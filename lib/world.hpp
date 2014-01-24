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
        string name;
        vector<agent*> agents;
        graph* g;
        int num_experiments;
        bool random_positions;
        agent* target;
    
    public:
        world(graph* g);
        world(char* filename);
        
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