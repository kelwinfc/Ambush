#include "world.hpp"

world::world()
{
    this->g = 0;
}
        
void world::add_agent(agent* a)
{
    this->agents.push_back(a);
}

float world::ambush_rate(agent* target)
{
    vector< pair<int, float> >* pred = 
        this->g->get_predecessors(target->get_current_vertex());
    vector< agent* >::iterator it;
    set<int> activated_pred;
    
    this->compute_paths(target);
    
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        vector<int>* path = a->get_path();
        if ( path->size() > 1 ){
            activated_pred.insert( path->at( path->size() - 2 ) );
        }
    }
    
    return activated_pred.size() / min(pred->size(), this->agents.size());
}

void world::compute_paths(agent* target)
{
    vector<agent*>::iterator it;
    for ( it = this->agents.begin(); it != this->agents.end(); ++it ){
        agent* a = *it;
        vector<int> path;
        a->set_path(path);
        a->get_behaviour()->get_plan(a, path);
        a->set_path(path);
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