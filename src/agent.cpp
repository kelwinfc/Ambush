#include "agent.hpp"

agent::agent(int index, graph* g, world* w, behavior* b, int current_v)
{
    this->index      = index;
    this->g          = g;
    this->w          = w;
    this->b          = b;
    this->v          = current_v;
    this->b_has_path = false;
}

agent::agent(const agent& a)
{
    this->index      = a.index;
    this->g          = a.g;
    this->w          = a.w;
    this->b          = a.b;
    this->v          = a.v;
    this->b_has_path = a.b_has_path;
}

int agent::get_index() const
{
    return this->index;
}

void agent::set_current_vertex(int v)
{
    this->v = v;
}

void agent::set_path(vector<int>& path)
{
    this->path       = path;
    this->b_has_path = true;
}

void agent::set_target(agent* a)
{
    this->target = a;
}

void agent::set_behavior(behavior* b)
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

behavior* agent::get_behavior()
{
    return this->b;
}

graph* agent::get_graph()
{
    return this->g;
}

void agent::clear_path()
{
    this->path.clear();
    this->b_has_path = false;
}

bool agent::has_path()
{
    return this->b_has_path;
}