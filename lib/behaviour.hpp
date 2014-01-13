#ifndef __AMBUSH_BEHAVIOUR
#define __AMBUSH_BEHAVIOUR

#include <vector>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"

using namespace std;

class agent;

class behaviour {
    
    public:
        behaviour(){}
        
        virtual int next_step(agent* a){ return -1; }
};

class noop : public behaviour {
    
    public:
        noop();
        virtual int next_step(agent* a);
};

// TODO
class ambush : public behaviour {
    
    public:
        ambush(){}
        
        virtual int next_step(agent* a){ return -1; }
};

// TODO
class priority_ambush : public behaviour {
    
    public:
        priority_ambush(){}
        
        virtual int next_step(agent* a){ return -1; }
};

// TODO
class r_ambush : public behaviour {
    
    public:
        r_ambush();
        
        virtual int next_step(agent* a){ return -1; }
};

// TODO
class self_adaptive_r_ambush : public behaviour {
    
    public:
        self_adaptive_r_ambush(){}
        
        virtual int next_step(agent* a){ return -1; }
};

// Experimentos de Kelwin
class partial_observavility_ambush : public behaviour {
    
    public:
        partial_observavility_ambush(){}
        
        virtual int next_step(agent* a){ return -1; }
};

// Tesis de Lezama
class capacity_ambush : public behaviour {
    
    public:
        capacity_ambush(){}
        
        virtual int next_step(agent* a){ return -1; }
};

#endif