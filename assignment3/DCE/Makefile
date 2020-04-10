INC=-I/usr/local/include/ -I ../DataflowFramework/include/ -I ./include/
SOURCES:= $(shell find ../DataflowFramework -type f -name '*.cpp')
OBJECTS:= $(SOURCES:.cpp=.o)
all: dce.so faintness.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

../DataflowFramework/%.o: ../DataflowFramework/%.cpp

dce.so: DCE.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

faintness.so: Faintness.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

# Uncomment and fix targets below after adding test cases
# Run
run_faintness: ./tests/calctest_m2r.bc all
	opt -load ./faintness.so -faintness $< -o /dev/zero 

run_dce: ./tests/calctest_m2r.bc all
	opt -load ./dce.so -dce $< -o /dev/zero

## Utils to view CFG, requires xdot and graphviz:
cfg_calctest: ./tests/calctest_m2r.bc
	opt -view-cfg $^

#cfg_live: ./tests/liveness-test-m2r.bc
#	opt -view-cfg $^
.PHONY: clean all
