#include "utils.hpp"

bool operator<(const astar_node& a, const astar_node& b)
{
    return a.f > b.f;
}

void concatenate_paths(vector<int>& a, vector<int>& b, int i, vector<int>& dst)
{
    dst.clear();
    
    for (int x = 0; x < i; x++ ){
        dst.push_back(a[x]);
    }
    for (unsigned int x = 0; x < b.size(); x++ ){
        dst.push_back(b[x]);
    }
}