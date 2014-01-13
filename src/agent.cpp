#include "agent.hpp"

int index;
int current_position;
vector<int> path;
agent* target;
behaviour* b;

agent::agent(int index, behaviour* b, int current_position)
{
    this->index            = index;
    this->b                = b;
    this->current_position = current_position;
}

void agent::set_current_position(int p)
{
    this->current_position = p;
}

void agent::set_path(vector<int>& path)
{
    this->path = path;
}

void agent::set_target(agent* a)
{
    this->target = a;
}

void agent::set_behaviour(behaviour* b)
{
    this->b = b;
}

int agent::get_current_position()
{
    return this->current_position;
}

vector<int>* agent::get_path()
{
    return &this->path;
}

agent* agent::get_target()
{
    return this->target;
}

behaviour* agent::get_behaviour()
{
    return this->b;
}
