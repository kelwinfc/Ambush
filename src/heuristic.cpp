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
    
    vx = this->g->get_arg(v, "x");
    vy = this->g->get_arg(v, "y");
    tx = this->g->get_arg(t, "x");
    ty = this->g->get_arg(t, "y");
    
    dx = vx - vy;
    dy = tx - ty;
    
    return sqrt( dx*dx + dy*dy );
}
