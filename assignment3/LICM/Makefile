INC=-I/usr/local/include/ -I ../DataflowFramework/include/ -I ./
SOURCES:= $(shell find ../DataflowFramework -type f -name '*.cpp')
OBJECTS:= $(SOURCES:.cpp=.o)
all: licm.so dominators.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

../DataflowFramework/%.o: ../DataflowFramework/%.cpp

%.so: %.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

# Uncomment after adding tests
# Run
#run_avail: ./tests/available-test-m2r.bc all
#	opt -load ./available.so -available $< -o /dev/zero 
#
#run_live: ./tests/liveness-test-m2r.bc all
#	opt -load ./liveness.so -liveness $< -o /dev/zero
#
## Utils to view CFG, requires xdot and graphviz:
#cfg_avail: ./tests/available-test-m2r.bc
#	opt -view-cfg $^
#
#cfg_live: ./tests/liveness-test-m2r.bc
#	opt -view-cfg $^
.PHONY: clean all
