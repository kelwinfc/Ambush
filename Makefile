GCC=g++
GCC_FLAGS=-Ilib -Wall -O3

LBOOST_FLAGS=-lboost_system -lboost_filesystem
OPENCV_FLAGS= `pkg-config opencv --cflags --libs` $(LBOOST_FLAGS) -Ilib -Icontrib -Wall -O3

FILES=utils args_manager heuristic graph node_selector behavior path_knowledge\
	agent world communication_function
EXECUTABLES=tests get_walkable_regions graph_from_walkable diff_ambush_density\
	eval_partial_communication

DEP_utils=
DEP_args_manager=
DEP_path_knowledge=utils
DEP_communication_function=utils path_knowledge
DEP_graph=utils args_manager
DEP_heuristic=utils agent graph world args_manager
DEP_world=utils agent graph behavior args_manager
DEP_node_selector=utils agent graph args_manager
DEP_behavior=path_knowledge communication_function agent utils heuristic graph\
	world args_manager
DEP_agent=utils behavior world args_manager path_knowledge 
DEP_tests=utils agent behavior graph world args_manager
DEP_diff_ambush_density=utils agent behavior graph world args_manager
DEP_get_walkable_regions=
DEP_graph_from_walkable=
DEP_eval_partial_communication=utils agent behavior graph world args_manager

all: $(EXECUTABLES)

tests: $(FILES:%=bin/%.o) bin/tests.o
	$(GCC) $^ -o $@ $(GCC_FLAGS)
diff_ambush_density: $(FILES:%=bin/%.o) bin/diff_ambush_density.o
	$(GCC) $^ -o $@ $(GCC_FLAGS)
get_walkable_regions: $(FILES:%=bin/%.o) bin/get_walkable_regions.o
	$(GCC) $^ -o $@ $(OPENCV_FLAGS)
graph_from_walkable: $(FILES:%=bin/%.o) bin/graph_from_walkable.o
	$(GCC) $^ -o $@ $(OPENCV_FLAGS)
eval_partial_communication: $(FILES:%=bin/%.o) bin/eval_partial_communication.o
	$(GCC) $^ -o $@ $(OPENCV_FLAGS)

#General rule for compiling
bin/%.o: src/%.cpp lib/%.hpp
	$(GCC) -c $< -o $@ $(GCC_FLAGS)

bin/utils.o: $(DEP_utils:%=src/%.cpp) $(DEP_utils:%=lib/%.hpp)
bin/args_manager.o: $(DEP_args_manager:%=src/%.cpp) $(DEP_args_manager:%=lib/%.hpp)
bin/graph.o: $(DEP_graph:%=src/%.cpp) $(DEP_graph:%=lib/%.hpp)
bin/node_selector.o: $(DEP_node_selector:%=src/%.cpp) $(DEP_node_selector:%=lib/%.hpp)
bin/behavior.o: $(DEP_behavior:%=src/%.cpp) $(DEP_behavior:%=lib/%.hpp)
bin/path_knowledge.o: $(DEP_path_knowledge:%=src/%.cpp) $(DEP_path_knowledge:%=lib/%.hpp)
bin/agent.o: $(DEP_agent:%=src/%.cpp) $(DEP_agent:%=lib/%.hpp)
bin/world.o: $(DEP_world:%=src/%.cpp) $(DEP_world:%=lib/%.hpp)
bin/heuristic.o: $(DEP_heuristic:%=src/%.cpp) $(DEP_heuristic:%=lib/%.hpp)
bin/communication_function.o: $(DEP_communication_function:%=src/%.cpp) $(DEP_communication_function:%=lib/%.hpp)

bin/tests.o: $(DEP_tests:%=src/%.cpp) $(DEP_tests:%=lib/%.hpp)
bin/diff_ambush_density.o: $(DEP_diff_ambush_density:%=src/%.cpp) $(DEP_diff_ambush_density:%=lib/%.hpp)
bin/get_walkable_regions.o: $(DEP_get_walkable_regions:%=src/%.cpp) $(DEP_get_walkable_regions:%=lib/%.hpp)
bin/graph_from_walkable.o: $(DEP_graph_from_walkable:%=src/%.cpp) $(DEP_graph_from_walkable:%=lib/%.hpp)
bin/eval_partial_communication.o: $(DEP_eval_partial_communication:%=src/%.cpp) $(DEP_eval_partial_communication:%=lib/%.hpp)

clean:
	rm -rf *~ */*~ */*/*~ *.pyc */*.pyc $(EXECUTABLES) bin/*.o

count_lines:
	wc -l src/*.cpp src/*.py lib/*.hpp other/*.py other/*.cpp | sort -gk 1
