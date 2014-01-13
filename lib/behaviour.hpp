#ifndef __AMBUSH_BEHAVIOUR
#define __AMBUSH_BEHAVIOUR

#include <vector>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"

using namespace std;

class agent;

//TODO define the basic functions to define a behaviour
class behaviour {
    
    public:
        behaviour();
        
        virtual int next_step(agent* a){ return -1; }
};

class noop {
    
    public:
        noop();
        virtual int next_step(agent* a);
};

#endif