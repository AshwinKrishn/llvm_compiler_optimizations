all : loop_test

loop_test:
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c loop_test.c -o loop_test.bc
	opt -mem2reg loop_test.bc -o loop_test.bc

