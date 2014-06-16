#include "world.hpp"
#include <iostream>

world::world(graph* g)
{
    this->g = g;
    this->name = "";
    this->num_experiments = 0;
    this->random_positions = true;
    this->target = 0;    
}

world::world(char* filename)
{
    FILE * pFile = fopen (filename, "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document document;
    document.ParseStream<0>(is);
    
    if ( document["name"].IsString() ){
        this->name = document["name"].GetString();
    }
    
    if ( document["num_experiments"].IsInt() ){
        this->num_experiments = document["num_experiments"].GetInt();   
    }
    
    if ( document["random_positions"].IsBool() ){
        this->num_experiments = document["random_positions"].GetBool();   
    }
    
    if ( document["graph"].IsString() ){
        this->g = new graph(document["graph"].GetString());
    }
    
    if ( document["target"].IsObject() ){
        this->target = new agent(document["target"]);
    }
    
    const rapidjson::Value& agents_json = document["agents"];
    if(agents_json.IsArray()){
        for (rapidjson::SizeType i = 0; i < agents_json.Size(); i++){
            const rapidjson::Value& next_agent = agents_json[i];
            
            if ( next_agent.IsObject() ){
                agent* a = new agent(next_agent);
                a->set_graph(this->g);
                a->set_world(this);
                a->set_target(this->target);
                this->add_agent(a);
            }
        }
    }
}

void world::add_agent(agent* a)
{
    this->agents.push_back(a);
}

void world::clear_paths()
{
    vector< agent* >::iterator it;
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        a->clear_path();
    }
}

float world::ambush_rate_relaxed(agent* target)
{
    vector< edge >* pred = 
        this->g->get_predecessors(target->get_current_vertex());
    
    int den = min(pred->size(), this->agents.size());
    
    if ( den == 0 ){
        return 1.0;
    }
    
    return (float) this->num_activated_predecessors(target) / (float) den;
}

float world::increment_rate(agent* target)
{
    vector<float> shortest_paths;
    vector<float> chosen_path;
    vector< agent* >::iterator it;
    float avg_increment = 0.0;
    
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        graph* g = a->get_graph();
        vector<int> path;
        
        a_star astar(this, 0);
        astar.get_plan(a, path);
        
        shortest_paths.push_back(g->path_cost(path));
        
        vector<int>* chosen = a->get_path();
        chosen_path.push_back(g->path_cost(*chosen));
    }
    
    for ( uint i=0; i<shortest_paths.size(); i++ ){
        if ( shortest_paths[i] == 0.0 ){
            avg_increment += 1.0;
        } else {
            avg_increment += chosen_path[i] / ( shortest_paths[i]);
        }
    }
    
    return ( avg_increment / this->agents.size() - 1.0 ) * 100.0;
}

float world::graph_coverage()
{
    set<int> covered_vertex;
    
    vector< agent* >::iterator it;
    
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        vector<int>* path = a->get_path();
        vector<int>::iterator path_it;
        for ( path_it = path->begin(); path_it != path->end(); ++path_it ){
            covered_vertex.insert( *path_it );
        }
    }
    
    return (float) covered_vertex.size() / (float) this->g->num_vertex();
}

void world::compute_paths(agent* target)
{
    vector<agent*>::iterator it;
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        
        if ( !a->has_path() ){
            vector<int> path;
            a->get_behavior()->get_plan(a, path);
            a->set_path(path);
        }
    }
}

vector<agent*>* world::get_agents()
{
    return &this->agents;
}

int world::num_agents()
{
    return this->agents.size();
}

graph* world::get_graph()
{
    return this->g;
}

int world::num_activated_predecessors(agent* target)
{
    set<int> activated_pred;
    vector< agent* >::iterator it;
    int ret = 0;
    
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        vector<int>* path = a->get_path();
        
        if ( path->size() > 1 && 
             path->at( path->size() -1 ) != target->get_current_vertex()
           )
        {
            continue;
        }
        
        if ( path->size() > 1 && path->at(0) != path->at(1) ){
            if ( activated_pred.insert( path->at( path->size() - 2 ) ).second )
                ret++;
        }
    }
    
    return ret;
}

float world::ambush_rate(agent* target)
{
    map<int, int> src_mapping;
    map<int, int> dst_mapping;
    vector< pair<int, int> > mm;
    
    graph* bip_graph = this->build_bipartite_reachability_graph(target,
        src_mapping, dst_mapping);

    float res = bip_graph->maximum_bipartite_matching(mm);
    
    float den = (float)mm.size();
    
    delete bip_graph;
    
    if ( mm.size() == 0 ){
        return 1.0;
    }
    
    return (float) this->num_activated_predecessors(target) / (float) den;
}

graph* world::build_bipartite_reachability_graph(agent* target,
                                                 map<int, int>& src_mapping,
                                                 map<int, int>& dst_mapping)
{
    graph* rg = new graph();
    rg->add_vertex(); // Super-source (index 0)
    rg->add_vertex(); // Super-sink (index 1)
    vector< agent* >::iterator it;
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        int agent_index = a->get_index();
        set<int> reachable_predecessors;
        int from, to;
        
        // Add each agent as source vertex
        from = src_mapping[agent_index] = rg->add_vertex() - 1;
        edge source_to_agent(0, from, 1.0);
        rg->add_edge(source_to_agent);
        
        /* Add the nodes reached by this agent (in the reduced graph) as
         * destination vertex
         */
        a->get_graph()->get_reachable_predecessors(a->get_current_vertex(),
                a->get_target()->get_current_vertex(), reachable_predecessors);
        
        set<int>::iterator pit;
        for ( pit = reachable_predecessors.begin();
              pit != reachable_predecessors.end(); ++pit )
        {
            if ( dst_mapping.find(*pit) == dst_mapping.end() ){
                to = dst_mapping[*pit] = rg->add_vertex() - 1;
                edge destination_to_sink(to, 1, 1.0);
                rg->add_edge(destination_to_sink);
            }
            
            to = dst_mapping[*pit];
            
            edge e(from, to, 1.0);
            rg->add_edge(e);
        }
    }
    return rg;
}
