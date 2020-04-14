INC=-I/usr/local/include/ -I ../DataflowFramework/include/ -I ./include/
SOURCES:= $(shell find ../DataflowFramework -type f -name '*.cpp')
OBJECTS:= $(SOURCES:.cpp=.o)
#DEBUG:= -DDEBUG_BUILD

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC $(DEBUG) 

all: dce.so faintness.so

./tests/%_m2r.bc: ./tests/%.c
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c $^ -o $(^:.c=.bc)
	opt -mem2reg $(^:.c=.bc) -o $@

../DataflowFramework/%.o: ../DataflowFramework/%.cpp

dce.so: DCE.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

faintness.so: Faintness.o $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so tests/*.bc

# Uncomment and fix targets below after adding test cases
# Run
run_faintness: ./tests/calctest_m2r.bc all
	opt -load ./faintness.so -faintness $< -o /dev/zero 

run_dce: ./tests/calctest_m2r.bc all
	opt -load ./faintness.so -load ./dce.so -dce_pass $< -o $(<:.bc=_mod.bc)

## Utils to view CFG, requires xdot and graphviz:
cfg_calctest: ./tests/calctest_m2r.bc
	opt -view-cfg $^

cfg_mod_calctest: ./tests/calctest_m2r_mod.bc
	opt -view-cfg $^

#cfg_live: ./tests/liveness-test-m2r.bc
#	opt -view-cfg $^
.PHONY: clean all
