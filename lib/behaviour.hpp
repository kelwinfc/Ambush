#ifndef __AMBUSH_BEHAVIOUR
#define __AMBUSH_BEHAVIOUR

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"
#include "world.hpp"
#include "heuristic.hpp"

using namespace std;

class agent;
class world;
class heuristic;

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

class randomized_dfs : public behaviour {
    
    public:
        randomized_dfs(world* w = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class increment_a_star : public behaviour {
    
    protected:
        heuristic* h;
        void (*increment_function)(world*, agent*, vector<float>&);
    
    public:
        increment_a_star(){ this->h = 0; this->increment_function = 0; }
        
        increment_a_star(void (*increment_function)(world*, agent*,
                                                    vector<float>&),
                         world* w = 0, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class a_star : public increment_a_star {
    
    public:
        a_star(world* w = 0, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class ambush : public increment_a_star {
    
    public:
        ambush(world* w = 0, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// TODO
class priority_ambush : public behaviour {
    
    private:
        heuristic* h;
    
    public:
        priority_ambush(world* w = 0, heuristic* h = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// TODO
class r_ambush : public behaviour {
    
    private:
        heuristic* h;
    
    public:
        r_ambush(world* w = 0, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// TODO
class self_adaptive_r_ambush : public behaviour {
    
    private:
        heuristic* h;
    
    public:
        self_adaptive_r_ambush(world* w = 0, heuristic* h = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// Experimentos de Kelwin
class partial_observability_ambush : public behaviour {
    
    private:
        heuristic* h;
    
    public:
        partial_observavility_ambush(world* w = 0, heuristic* h = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// Tesis de Lezama
class capacity_ambush : public behaviour {
    
    private:
        heuristic* h;
    
    public:
        capacity_ambush(world* w = 0, heuristic* h = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

#endif