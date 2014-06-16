#include "agent.hpp"

agent::agent(int index, graph* g, world* w, behavior* b, int current_v)
{
    this->index      = index;
    this->g          = g;
    this->w          = w;
    this->b          = b;
    this->v          = current_v;
    this->b_has_path = false;
    this->area       = 72; //zelda
    //this->area       = 9.28; //blocks
    this->max_speed  = 1.4;
    this->simulation_vertex = current_v;
}

agent::agent(const agent& a)
{
    this->index             = a.index;
    this->g                 = a.g;
    this->w                 = a.w;
    this->b                 = a.b;
    this->v                 = a.v;
    this->b_has_path        = a.b_has_path;
    this->area              = a.area;
    this->max_speed         = a.max_speed;
    this->simulation_vertex = a.simulation_vertex;
}

agent::agent(const rapidjson::Value& description)
{
    if ( description.IsArray() ){
        for (rapidjson::SizeType j = 0; j < description.Size(); j++){
            
            const rapidjson::Value& descr = description[j];
            if ( descr["pos"].IsInt() ){
                this->v = descr["pos"].GetInt();
            }
            
            if ( descr["args"].IsObject() ){
                args_manager args(descr["args"]);
                this->args = args;
            }
        }
    }
}

void agent::set_world(world* w)
{
    this->w = w;
}

void agent::set_graph(graph* g)
{
    this->g = g;
}

int agent::get_index() const
{
    return this->index;
}

void agent::set_current_vertex(int v)
{
    this->v = v;
}

void agent::set_current_simulation_vertex(int v)
{
    this->simulation_vertex = v;
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

int agent::get_current_simulation_vertex()
{
    return this->simulation_vertex;
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

float agent::get_max_speed()
{
    return this->max_speed;
}

float agent::get_capacity()
{
     return this->area;
}