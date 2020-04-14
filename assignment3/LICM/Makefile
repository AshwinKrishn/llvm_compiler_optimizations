INC=-I/usr/local/include/ -I ../DataflowFramework/include/ -I ./include/
SOURCES:= $(shell find ../DataflowFramework -type f -name '*.cpp')
OBJECTS:= $(SOURCES:.cpp=.o)
all: licm.so dominators.so reachingdefs.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

../DataflowFramework/%.o: ../DataflowFramework/%.cpp

licm.so: LICM.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

dominators.so: DominatorPass.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

reachingdefs.so: ReachingDefinitions.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

# Run
run_licm: ./tests/loop_test.bc all
	opt -load=./reachingdefs.so -load=./dominators.so -load=./licm.so -licm_pass $< -o /dev/zero 

#run_live: ./tests/liveness-test-m2r.bc all
#	opt -load ./liveness.so -liveness $< -o /dev/zero
#

run_rd: ./tests/rdtest_m2r.bc all
	opt -load=./reachingdefs.so -rd $< -o /dev/zero 

# Utils to view CFG, requires xdot and graphviz:
cfg_rd: ./tests/rdtest_m2r.bc
	opt -view-cfg $^

#cfg_live: ./tests/liveness-test-m2r.bc
#	opt -view-cfg $^
.PHONY: clean all
