
#include "compiler/argread.h"
#include "compiler/compiler.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
    // Compile the code inside the file
    Compiler comp;
    ArgReader argReader(&comp);
    argReader.read(argc, argv);
    comp.compile();
    return 0;
}