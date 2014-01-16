#ifndef __AMBUSH_UTILS
#define __AMBUSH_UTILS

struct astar_node {
    int v; // Current node
    int p; // Node's parent
    int f; // Estimated cost from the source to the goal
    int g; // Accumulated cost from the source to v
};

bool operator<(const astar_node& a, const astar_node& b);

#endif