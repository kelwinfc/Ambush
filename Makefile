GCC=g++
GCC_FLAGS=-Ilib -Wall -O3

FILES=utils args_manager heuristic graph node_selector behavior agent world
EXECUTABLES=tests

DEP_utils=
DEP_args_manager=
DEP_graph=utils args_manager
DEP_heuristic=utils agent graph world args_manager
DEP_world=utils agent graph behavior args_manager
DEP_node_selector=utils agent graph args_manager
DEP_behavior=agent utils heuristic graph world args_manager
DEP_agent=utils behavior world args_manager
DEP_tests=utils agent behavior graph world args_manager
all: $(EXECUTABLES)

tests: $(FILES:%=bin/%.o) bin/tests.o
	$(GCC) $^ -o $@ $(GCC_FLAGS)

#General rule for compiling
bin/%.o: src/%.cpp lib/%.hpp
	$(GCC) -c $< -o $@ $(GCC_FLAGS)

bin/utils.o: $(DEP_utils:%=src/%.cpp) $(DEP_utils:%=lib/%.hpp)
bin/args_manager.o: $(DEP_args_manager:%=src/%.cpp) $(DEP_args_manager:%=lib/%.hpp)
bin/graph.o: $(DEP_graph:%=src/%.cpp) $(DEP_graph:%=lib/%.hpp)
bin/node_selector.o: $(DEP_node_selector:%=src/%.cpp) $(DEP_node_selector:%=lib/%.hpp)
bin/behavior.o: $(DEP_behavior:%=src/%.cpp) $(DEP_behavior:%=lib/%.hpp)
bin/agent.o: $(DEP_agent:%=src/%.cpp) $(DEP_agent:%=lib/%.hpp)
bin/world.o: $(DEP_world:%=src/%.cpp) $(DEP_world:%=lib/%.hpp)
bin/heuristic.o: $(DEP_heuristic:%=src/%.cpp) $(DEP_heuristic:%=lib/%.hpp)

bin/tests.o: $(DEP_tests:%=src/%.cpp) $(DEP_tests:%=lib/%.hpp)

clean:
	rm -rf *~ */*~ */*/*~ *.pyc */*.pyc $(EXECUTABLES) bin/*.o

count_lines:
	wc -l src/*.cpp src/*.py lib/*.hpp other/*.py other/*.cpp | sort -gk 1