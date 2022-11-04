all:
	@(cd ../../build && cmake --build . --target lifeline)

test:
	@(clang-14 -S -emit-llvm test.c && ./lifeline ./test.ll)