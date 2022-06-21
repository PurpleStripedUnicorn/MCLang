
# subfolders = lexer parser parsenodes bcgen
subfolders = $(subst mclang,,$(subst mclang/,,$(shell find mclang -type d)))
cppargs = -std=c++17 -Imclang -Wall -Wextra

buildfolders = $(addprefix build/,$(subfolders))
ccfiles = $(foreach dir,$(subfolders),$(shell find mclang/$(dir)/*.cc))
hfiles = $(ccfiles:.cc=.h)
ofiles = $(subst mclang,build,$(ccfiles:.cc=.o))

# Makefile starting point
.PHONY: all
all: build $(buildfolders) build/main

# Clean up build folder
clean:
	rm -r build/*
	rm *.debug

# Create output C++ files and use them to build main.cc
build/main: mclang/main.cc $(ofiles) $(hfiles)
	g++ $(cppargs) -o build/main mclang/main.cc $(ofiles)
build/%.o: mclang/%.cc mclang/%.h
	g++ $(cppargs) -o $@ -c $<

# Create folders if neccessary
build:
	mkdir build
build/%:
	mkdir $@