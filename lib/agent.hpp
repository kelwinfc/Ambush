#ifndef __AMBUSH_AGENT
#define __AMBUSH_AGENT

#include <vector>

#include "behaviour.hpp"
#include "utils.hpp"

using namespace std;

class behaviour;

class agent {
    private:
        int index;
        int current_position;
        vector<int> path;
        agent* target;
        behaviour* b;
        
        float speed;
        float capacity;
    
    public:
        agent(int index, behaviour* b = 0, int current_position = -1);
        
        void set_current_position(int p);
        void set_path(vector<int>& path);
        void set_target(agent* a);
        void set_behaviour(behaviour* b);
        
        int          get_current_position();
        vector<int>* get_path();
        agent*       get_target();
        behaviour*   get_behaviour();
};

#endif