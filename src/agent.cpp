#include "agent.hpp"

agent::agent(int index, graph* g, world* w, behaviour* b, int current_v)
{
    this->index = index;
    this->g     = g;
    this->w     = w;
    this->b     = b;
    this->v     = current_v;
}

void agent::set_current_vertex(int v)
{
    this->v = v;
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

int agent::get_current_vertex()
{
    return this->v;
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

graph* agent::get_graph()
{
    return this->g;
}