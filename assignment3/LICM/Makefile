INC=-I/usr/local/include/ -I ../DataflowFramework/include/ -I ./include/
SOURCES:= $(shell find ../DataflowFramework -type f -name '*.cpp')
OBJECTS:= $(SOURCES:.cpp=.o)
all: licm.so dominators.so reachingdefs.so landingpad.so make_test

make_test:
	make -C tests/
CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

../DataflowFramework/%.o: ../DataflowFramework/%.cpp

licm.so: LICM.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

dominators.so: DominatorPass.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

reachingdefs.so: ReachingDefinitions.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

landingpad.so: LandingPadTransform.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so tests/*.bc

# Run
run_licm:  all
	opt -load=./landingpad.so -landing-pad ./tests/loop_test.bc -o tests/loop_test_mod.bc
	opt -load=./landingpad.so -load=./reachingdefs.so -load=./dominators.so -load=./licm.so -licm_pass tests/loop_test_mod.bc -o tests/loop_invarients.bc
	opt -view-cfg tests/loop_invarients.bc 

run_licm_sefat:  all
	opt -load=./landingpad.so -landing-pad ./tests/sefat_test_m2r.bc -o tests/sefat_test_lp.bc
	opt -load=./landingpad.so -load=./reachingdefs.so -load=./dominators.so -load=./licm.so -licm_pass tests/sefat_test_lp.bc -o tests/sefat_invarients.bc
	opt -view-cfg tests/sefat_invarients.bc

run_landing: ./tests/loop_test.bc all
	opt -load=./landingpad.so -landing-pad $< -o tests/loop_test_mod.bc

run_rd: ./tests/rdtest_m2r.bc all
	opt -load=./reachingdefs.so -rd $< -o /dev/zero 

# Utils to view CFG, requires xdot and graphviz:
cfg_rd: ./tests/rdtest_m2r.bc
	opt -view-cfg $^

cfg_loop: ./tests/loop_test.bc
	opt -view-cfg $^

cfg_loop_mod: ./tests/loop_test_mod.bc
	opt -view-cfg $^
.PHONY: clean all
