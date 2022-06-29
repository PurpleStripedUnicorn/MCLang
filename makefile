
ccflags =

# Detect operating system
ifeq ($(OS),Windows_NT)
ccflags += -D WINDOWS
preps = $(shell echo .\)
delim = $(shell echo \)
subfolders = $(subst $(CURDIR)/mclang/,,$(subst \,/,$(shell dir /b /s /ad mclang)))
cppargs = -std=c++17 -Imclang -Wall -Wextra
buildfolders = $(addprefix build/,$(subfolders))
ccfiles = $(foreach sub,$(subfolders),$(subst $(CURDIR)/,,$(subst \,/,$(shell dir /b /s /a mclang\$(sub)\*.cc))))
hfiles = $(ccfiles:.cc=.h)
ofiles = $(subst mclang,build,$(ccfiles:.cc=.o))
cc = g++
else ifeq ($(shell uname -s),Linux)
delim = /
ccflags += -D LINUX
preps = 
subfolders = $(subst mclang,,$(subst mclang/,,$(shell find mclang -type d)))
cppargs = -std=c++17 -Imclang -Wall -Wextra
buildfolders = $(addprefix build/,$(subfolders))
ccfiles = $(foreach dir,$(subfolders),$(shell find mclang/$(dir)/*.cc))
hfiles = $(ccfiles:.cc=.h)
ofiles = $(subst mclang,build,$(ccfiles:.cc=.o))
cc = g++
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
	rm -r build$(delim)*

# Clean up output datapack and debug files
clean_dp:
	rm -r out_datapack
	rm *.debug

# Create output C++ files and use them to build main.cc
build/main: mclang/main.cc $(ofiles) $(hfiles)
	$(cc) $(cppargs) $(ccflags) -o $(preps)build$(delim)main $(preps)mclang$(delim)main.cc $(preps)$(subst /,$(delim),$(ofiles))
build/%.o: mclang/%.cc mclang/%.h
	$(cc) $(cppargs) $(ccflags) -o $(preps)$(subst /,$(delim),$@) -c $(preps)$(subst /,$(delim),$<)

# Create folders if neccessary
build:
	mkdir build
build/%:
	mkdir $(subst /,$(delim),$@)