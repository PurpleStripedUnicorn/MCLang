
os ?= LINUX
subfolders = $(subst mclang,,$(subst mclang/,,$(shell find mclang -type d)))
cppargs = -std=c++17 -Imclang -Wall -Wextra
buildfolders = $(addprefix build/,$(subfolders))
ccfiles = $(foreach dir,$(subfolders),$(shell find mclang/$(dir)/*.cc))
hfiles = $(ccfiles:.cc=.h)
ofiles = $(subst mclang,build,$(ccfiles:.cc=.o))

ifeq ($(os),LINUX)
	cc = g++
else ifeq ($(os),WINDOWS)
	cc = x86_64-w64-mingw32-g++
	cppargs += -static
else
	exit 1
endif

# Makefile starting point
.PHONY: all
all: build $(buildfolders) build/main

# Clean up build fodler and output datapack
clean: clean_build clean_dp

# Clean up build folder
clean_build:
	rm -r build/*

# Clean up output datapack and debug files
clean_dp:
	rm -r out_datapack
	rm *.debug

# Create output C++ files and use them to build main.cc
build/main: mclang/main.cc $(ofiles) $(hfiles)
	$(cc) $(cppargs) -o build/main mclang/main.cc $(ofiles)
build/%.o: mclang/%.cc mclang/%.h
	$(cc) $(cppargs) -o $@ -c $<

# Create folders if neccessary
build:
	mkdir build
build/%:
	mkdir $@