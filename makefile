
subfolders = lexer parser parsenodes
cppargs = -Imclang -Wall -Wextra

buildfolders = $(addprefix build/,$(subfolders))
ccfiles = $(foreach dir,$(subfolders),$(shell find mclang/$(dir)/*.cc))
tmpofiles = $(ccfiles:.cc=.o)
ofiles = $(subst mclang,build,$(tmpofiles))

# Makefile starting point
.PHONY: all
all: build $(buildfolders) build/main

# Clean up build folder
clean:
	rm -r build/*

# Create output C++ files and use them to build main.cc
build/main: $(ofiles)
	g++ $(cppargs) -o build/main mclang/main.cc $^
build/%.o: mclang/%.cc
	g++ $(cppargs) -o $@ -c $<

# Create folders if neccessary
build:
	mkdir build
build/%:
	mkdir $@