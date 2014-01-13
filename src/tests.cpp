#include "tests.hpp"

int main(int argc, char* argv[])
{
    noop np;
    agent a(1, &np, 0);
    return 0;
}