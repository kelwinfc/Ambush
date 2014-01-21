#ifndef __AMBUSH_BEHAVIOUR
#define __AMBUSH_BEHAVIOUR

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>

#include "graph.hpp"
#include "utils.hpp"
#include "agent.hpp"
#include "world.hpp"
#include "node_selector.hpp"
#include "heuristic.hpp"

using namespace std;

class agent;
class world;
class heuristic;
class node_selector;

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
        noop(world* w);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class randomized_dfs : public behaviour {
    
    public:
        randomized_dfs(world* w);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class increment_a_star : public behaviour {
    
    protected:
        heuristic* h;
        void (*I)(world*, agent*, vector<float>&);
        bool delete_h;
    
    public:
        increment_a_star();
        increment_a_star(void (*increment_function)(world*, agent*,
                                                    vector<float>&),
                         world* w, heuristic* h = 0);
        
        ~increment_a_star();
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class a_star : public increment_a_star {
    
    public:
        a_star(world* w, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class ambush : public increment_a_star {
    
    public:
        ambush(world* w, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

bool index_priority(agent* a, agent* b);

class priority_ambush : public increment_a_star {
    
    protected:
        bool (*P)(agent*, agent*);
        
    public:
        priority_ambush(world* w,
                        bool (*priority_function)(agent*, agent*) = 
                                                    &index_priority,
                        heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class self_adaptive_r_ambush : public increment_a_star {
    
    protected:
        node_selector* selector;
        bool delete_selector;
    
    public:
        self_adaptive_r_ambush();
        self_adaptive_r_ambush(world* w, node_selector* ns=0,
                               heuristic* h = 0);
        ~self_adaptive_r_ambush();
        
        // TODO
        virtual void get_plan(agent* a, vector<int>& path);
    
    protected:
        float uniformity_metric(int target);
        void get_pred_in_paths(set<int>& nodes, int target);
};

class r_ambush : public self_adaptive_r_ambush {
    
    protected:
        float r;
    
    public:
        r_ambush(world* w, float r, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// Experimentos de Kelwin
class partial_observability_ambush : public increment_a_star {
    
    protected:
        
    public:
        partial_observability_ambush(world* w, heuristic* h = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

// Tesis de Lezama
class capacity_ambush : public increment_a_star {
    
    protected:
        
    public:
        capacity_ambush(world* w, heuristic* h = 0){}
        
        virtual void get_plan(agent* a, vector<int>& path);
};

#endif