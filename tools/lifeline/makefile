all:
	@(cd ../../build && cmake --build . --target lifeline)

test: all
	@(clang-14 -S -emit-llvm test.c && ./lifeline ./test.ll)