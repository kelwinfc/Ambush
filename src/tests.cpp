#include "tests.hpp"

int main(int argc, char* argv[])
{
    world w;
    noop np;
    ambush amb(&w);
    graph g((char*)"test/graphs/g1.json");
    agent a(1, &g, &w, &amb, 0);
    return 0;
}