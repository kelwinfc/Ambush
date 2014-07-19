#ifndef __AMBUSH_COMMUNICATION
#define __AMBUSH_COMMUNICATION

#include <map>
#include <vector>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"
#include "world.hpp"

using namespace std;

class communication_function {
    protected:
        world* w;
    
    public:
        communication_function();
        communication_function(world* w);
        
        virtual bool can_communicate(agent* a, agent* b);
        void communicate(agent* a, agent* b);
};

class true_communication_function : public communication_function {
    public:
        true_communication_function();
        true_communication_function(world* w);
        
        virtual bool can_communicate(agent* a, agent* b);
};

class false_communication_function : public communication_function {
    public:
        false_communication_function();
        false_communication_function(world* w);
        
        virtual bool can_communicate(agent* a, agent* b);
};

#endif