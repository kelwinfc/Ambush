#include "world.hpp"

world::world()
{
    
}
        
void world::add_agent(agent* a)
{
    this->agents.push_back(a);
}

float world::ambush_rate(agent* target)
{
    return 0.0;
}

void world::compute_paths(agent* target)
{
    
}

vector<agent*>* world::get_agents()
{
    return &this->agents;
}

int world::num_agents()
{
    return this->agents.size();
}