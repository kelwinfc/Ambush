#include "utils.hpp"

bool operator<(const astar_node& a, const astar_node& b)
{
    return a.f > b.f;
}