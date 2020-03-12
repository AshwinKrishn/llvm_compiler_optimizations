INC=-I/usr/local/include/ -I ./DataflowFramework/include/
SOURCES:= $(shell find ./DataflowFramework -type f -name '*.cpp')
OBJECTS:= $(SOURCES:.cpp=.o)
all: liveness.so available.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

DataflowFramework/%.o: DataflowFramework/%.cpp

available-support.o: available-support.cpp available-support.h	

%.so: %.o $(OBJECTS) available-support.o
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

# Run
run_avail: ./tests/available-test-m2r.bc all
	opt -load ./available.so -available $< -o /dev/zero

run_live: ./tests/liveness-test-m2r.bc all
	opt -load ./liveness.so -liveness $< -o /dev/zero

# Utils to view CFG, requires xdot and graphviz:
cfg_avail: ./tests/available-test-m2r.bc
	opt -view-cfg $^

cfg_live: ./tests/liveness-test-m2r.bc
	opt -view-cfg $^
.PHONY: clean all
