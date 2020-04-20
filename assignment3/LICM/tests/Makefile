all : loop_test loop_test_sefat

loop_test:
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c loop_test.c -o loop_test.bc
	opt -mem2reg loop_test.bc -o loop_test.bc

loop_test_sefat:
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c sefat_test.c -o sefat_test.bc
	opt -mem2reg sefat_test.bc -o sefat_test_m2r.bc
