INC=-I/usr/local/include/ -I ./DataflowFramework/include/
all: liveness.so available.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

DataflowFramework/%.o: DataflowFramework/%.cpp

available-support.o: available-support.cpp available-support.h	

%.so: %.o DataflowFramework/dataflow.o available-support.o
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

.PHONY: clean all
