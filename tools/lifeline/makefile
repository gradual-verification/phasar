.PHONY: default
default: lifeline ;

test: lifeline test.ll
	@(clang-14 -S -emit-llvm test.c && ./lifeline ./test.ll)
test.ll:
	@(clang-14 -O0 -S -emit-llvm test.c )
lifeline:
	@(rm -f lifeline)
	@(cd ../../build && cmake --build . --target lifeline)
	@(ln -s ../../build/tools/lifeline/lifeline lifeline)
clean:
	@rm lifeline
	@rm *.ll