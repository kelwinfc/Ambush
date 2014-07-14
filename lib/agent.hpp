#ifndef __AMBUSH_AGENT
#define __AMBUSH_AGENT

#include <vector>

#include "behavior.hpp"
#include "utils.hpp"
#include "world.hpp"
#include "args_manager.hpp"
#include "path_knowledge.hpp"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

class behavior;
class world;

class agent {
    private:
        int index;
        int v;
        world* w;
        vector<int> path;
        agent* target;
        behavior* b;
        graph* g;
        args_manager args;
        
        float max_speed;
        int simulation_vertex;
        float area;
        
        bool b_has_path;
    
        path_knowledge* knowledge;
    
    public:
        agent(int index, graph* g, world* w, behavior* b,
              int current_v = -1);
        agent(const agent& a);
        agent(const rapidjson::Value& description);
        
        void set_world(world* w);
        void set_graph(graph* g);
        
        void set_current_vertex(int v);
        void set_current_simulation_vertex(int v);
        
        void set_path(vector<int>& path);
        void set_target(agent* a);
        void set_behavior(behavior* b);
        void clear_path();
        bool has_path();
        int          get_index() const;
        int          get_current_vertex();
        int          get_current_simulation_vertex();
        vector<int>* get_path();
        agent*       get_target();
        behavior*    get_behavior();
        graph*       get_graph();
        float        get_max_speed();
        float        get_capacity();
        
        void get_reachable_predecessors(int target, set<int>& predecessors);
};

#endif
