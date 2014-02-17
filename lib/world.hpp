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
        float ambush_rate_relaxed(agent* target);
        float ambush_rate(agent* target);
        
        float increment_rate(agent* target);
        float graph_coverage();
        
        graph* get_graph();

    private:
        int num_activated_predecessors(agent* target);

        /* Builds the bipartite graph that matches each agent to the reachable
         * predecessors of the target by him. Two artificial nodes are
         * created: a super-source (index 0) and a super-sink (index 1)
         */
        graph* build_bipartite_reachability_graph(agent* target,
                                                  map<int, int>& src_mapping,
                                                  map<int, int>& dst_mapping);
};

#endif
