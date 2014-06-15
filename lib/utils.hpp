#ifndef __AMBUSH_UTILS
#define __AMBUSH_UTILS

#include <vector>
#include <iostream>

using namespace std;

struct astar_node {
    int v; // Current node
    int p; // Node's parent
    float f; // Estimated cost from the source to the goal
    float g; // Accumulated cost from the source to v
};

bool operator<(const astar_node& a, const astar_node& b);

void concatenate_paths(vector<int>& a, vector<int>& b, int i, vector<int>& dst);

#endif