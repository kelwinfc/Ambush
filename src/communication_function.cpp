#include "communication_function.hpp"

communication_function::communication_function()
{
    this->w = 0;
}

communication_function::communication_function(world* w)
{
    this->w = w;
}

bool communication_function::can_communicate(agent* a, agent* b)
{
    return true;
}

void communication_function::communicate(agent* a, agent* b)
{
    //TODO
}

true_communication_function::true_communication_function()
{
    this->w = 0;
}

true_communication_function::true_communication_function(world* w)
{
    this->w = w;
}

bool true_communication_function::can_communicate(agent* a, agent* b)
{
    return true;
}

false_communication_function::false_communication_function()
{
    this->w = 0;
}

false_communication_function::false_communication_function(world* w)
{
    this->w = w;
}

bool false_communication_function::can_communicate(agent* a, agent* b)
{
    return false;
}
