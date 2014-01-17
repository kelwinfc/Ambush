#ifndef __AMBUSH_AGENT
#define __AMBUSH_AGENT

#include <vector>

#include "behaviour.hpp"
#include "utils.hpp"
#include "world.hpp"

using namespace std;

class behaviour;
class world;

class agent {
    private:
        int index;
        int v;
        world* w;
        vector<int> path;
        agent* target;
        behaviour* b;
        graph* g;
        
        float speed;
        float capacity;
        
        bool b_has_path;
    
    public:
        agent(int index, graph* g, world* w, behaviour* b,
              int current_v = -1);
        agent(const agent& a);
        
        void set_current_vertex(int v);
        void set_path(vector<int>& path);
        void set_target(agent* a);
        void set_behaviour(behaviour* b);
        void clear_path();
        bool has_path();
        
        int          get_index() const;
        int          get_current_vertex();
        vector<int>* get_path();
        agent*       get_target();
        behaviour*   get_behaviour();
        graph*       get_graph();
};

#endif