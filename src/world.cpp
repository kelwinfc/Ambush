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

float world::ambush_rate(agent* target)
{
    vector< edge >* pred = 
        this->g->get_predecessors(target->get_current_vertex());
    
    vector< agent* >::iterator it;
    set<int> activated_pred;
    
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        vector<int>* path = a->get_path();
        if ( path->size() > 1 ){
            activated_pred.insert( path->at( path->size() - 2 ) );
        }
    }
    
    int den = min(pred->size(), this->agents.size());
    if ( den == 0 ){
        return 1.0;
    }
    
    return (float) activated_pred.size() / (float) den;
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
        if ( shortest_paths[i] < 1e-6 ){
            continue;
        }
        avg_increment += chosen_path[i] * 100.0 / shortest_paths[i];
    }
    
    return avg_increment / this->agents.size() - 100.0;
}

float world::graph_coverage()
{
    cout << endl;
    set<int> covered_vertex;
    
    vector< agent* >::iterator it;
    
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        vector<int>* path = a->get_path();
        vector<int>::iterator path_it;
        for ( path_it = path->begin(); path_it != path->end(); ++path_it ){
            cout << *path_it << " ";
            covered_vertex.insert( *path_it );
        }
        cout << endl;
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
