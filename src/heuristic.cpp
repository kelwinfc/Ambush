#include "heuristic.hpp"

heuristic::heuristic()
{
    this->w = 0;
    this->g = 0;
}

heuristic::heuristic(world* w)
{
    this->w = w;
}

float heuristic::h(int v, int t)
{
    return 0.0;
}

h_zero::h_zero(world* w)
{
    this->w = w;
}
        
float h_zero::h(int v, int t)
{
    return 0.0;
}

h_euclidean::h_euclidean(world* w)
{
    this->w = w;
    this->g = g;
}

float h_euclidean::h(int v, int t)
{
    float vx, vy, tx, ty, dx, dy;
    node* am_v = this->g->get_node(v);
    node* am_t = this->g->get_node(t);
    
    vx = am_v->args.get_float_arg("x");
    vy = am_v->args.get_float_arg("y");
    tx = am_t->args.get_float_arg("x");
    ty = am_t->args.get_float_arg("y");
    
    dx = vx - vy;
    dy = tx - ty;
    
    return sqrt( dx*dx + dy*dy );
}
