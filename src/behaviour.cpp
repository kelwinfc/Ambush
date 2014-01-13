#include "behaviour.hpp"

noop::noop()
{
    
}

int noop::next_step(agent* a)
{
    return a->get_current_position();
}
