#ifndef __AMBUSH_BEHAVIOR
#define __AMBUSH_BEHAVIOR

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
#include "communication_function.hpp"

using namespace std;

class agent;
class world;
class heuristic;
class node_selector;
class communication_function;

class behavior {
    protected:
        world* w;
    
    public:
        behavior(){ this->w = 0; }
        behavior(world* w){ this->w = w; }
        
        int next_step(agent* a);
        
        virtual void get_plan(agent* a, vector<int>& path){}
};

class noop : public behavior {
    
    public:
        noop(world* w);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class randomized_dfs : public behavior {
    
    public:
        randomized_dfs(world* w);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class increment_a_star : public behavior {
    
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
        
        virtual void get_plan(agent* a, vector<int>& path);
    
    protected:
        
        /* This function computes the uniformity of the distribution of the
         * agents towards the target. The function returns a value between 0
         * and 1. A value close to 0 means that the distribution is uniforme,
         * a value close to 1 means the opposite.
         */
        float uniformity_metric(int target);
        
        /* This function computes the set of target's predecessors that belong
         * to a agent's path.
         */
        void get_pred_in_paths(set<int>& nodes, int target);
};

class r_ambush : public self_adaptive_r_ambush {
    
    protected:
        float r;
    
    public:
        r_ambush(world* w, float r, heuristic* h = 0);
        
        virtual void get_plan(agent* a, vector<int>& path);
};

class density_crowd : public behavior {
    
    protected:
        heuristic* h;
        float w_delay;
    
    public:
        density_crowd(world* w, heuristic* h = 0);
        ~density_crowd();
        
        virtual void get_plan(agent* a, vector<int>& path);
    
    protected:
        void save_initial_nodes(vector<int>& n);
        void restore_initial_nodes(vector<int>& n);
        void initialize_paths(vector< vector<int> >& paths);
        void set_paths(vector< vector<int> >& paths);
        int closest_agent_to_next_node();
        bool update_path(int next_agent,
                         vector< vector<int> >& accumulated_path_by_agent);
        int num_remaining_agents();
        
        bool get_paths();
        void get_density_path(agent* a, vector<int>& path);
};

class partial_communication : public increment_a_star {
    
    protected:
        communication_function* communication;
        bool delete_communication;
    
    public:
        partial_communication(world* w, communication_function* cf = 0,
                              heuristic* h = 0);
        ~partial_communication();
        
        virtual void get_plan(agent* a, vector<int>& path);
};

#endif
