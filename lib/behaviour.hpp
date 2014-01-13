#ifndef __AMBUSH_BEHAVIOUR
#define __AMBUSH_BEHAVIOUR

#include <vector>
#include <algorithm>
#include <queue>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"
#include "world.hpp"

using namespace std;

class agent;
class world;

class behaviour {
    protected:
        world* w;
    
    public:
        behaviour(){ this->w = 0; }
        behaviour(world* w){ this->w = w; }
        
        int next_step(agent* a);
        
        virtual void get_plan(agent* a, vector<int>& path){}
};

class noop : public behaviour {
    
    public:
        noop(world* w = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class ambush : public behaviour {
    
    public:
        ambush(world* w = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// TODO
class priority_ambush : public behaviour {
    
    public:
        priority_ambush(world* w = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// TODO
class r_ambush : public behaviour {
    
    public:
        r_ambush(world* w = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// TODO
class self_adaptive_r_ambush : public behaviour {
    
    public:
        self_adaptive_r_ambush(world* w = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// Experimentos de Kelwin
class partial_observavility_ambush : public behaviour {
    
    public:
        partial_observavility_ambush(world* w = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// Tesis de Lezama
class capacity_ambush : public behaviour {
    
    public:
        capacity_ambush(world* w = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

#endif