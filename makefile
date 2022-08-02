
os ?= LINUX
subfolders = $(subst mclang,,$(subst mclang/,,$(shell find mclang -type d)))
cppargs = -std=c++17 -Imclang -Wall -Wextra
buildfolders = $(addprefix build/,$(subfolders))
ccfiles = $(shell find mclang -name "*.cc") mclang/stdlib/stdlib.cc
hfiles = $(ccfiles:.cc=.h)
ofiles = $(subst mclang,build,$(ccfiles:.cc=.o))
dfiles = $(ofiles:.o=.d)
testfiles = $(shell find tests/*.cc)
testbuilds = $(subst .cc,,$(addprefix build/,$(testfiles)))

ifeq ($(os),LINUX)
	cc = g++
	cppargs += -DOS_LINUX
else ifeq ($(os),WINDOWS)
	cc = x86_64-w64-mingw32-g++
	cppargs += -static
	cppargs += -DOS_WINDOWS
else
	exit 1
endif

# Makefile starting point
.PHONY: all
all: build $(buildfolders) build/main $(testbuilds)

# Clean up build fodler and output datapack
clean: clean_build clean_dp

# Clean up build folder
clean_build:
	rm -r -f build/*
	rm -f mclang/stdlib/stdlib.cc
	rm -f mclang/stdlib/stdlib.h

# Clean up output datapack and debug files
clean_dp:
	rm -r -f out_datapack/*
	rm -f *.debug

# Create output C++ files and use them to build main.cc
build/main: $(ofiles)
	$(cc) $(cppargs) -o build/main $(ofiles)

# Dependency files
-include $(dfiles)

# Output C++ files
build/%.o: mclang/%.cc makefile mclang/stdlib/stdlib.cc mclang/stdlib/stdlib.h
	$(cc) $(cppargs) -MMD -MP -c $< -o $@

# Testing program
build/tests/%: tests/%.cc build/tests
	$(cc) $(cppargs) -o $@ $<
build/tests:
	mkdir build/tests

# Standard library
mclang/stdlib/stdlib.cc: mclang/stdlib/stdlib.h
	touch mclang/stdlib/stdlib.cc
mclang/stdlib/stdlib.h: build/stdlibexe
	build/stdlibexe
build/stdlibexe: stdlib/build.cc
	$(cc) $(cppargs) stdlib/build.cc -o build/stdlibexe

# Create folders if neccessary
build:
	mkdir build
$(buildfolders):
	mkdir $@