
#include "general/loc.h"

Loc::Loc(std::string filename, unsigned int line, unsigned int col) :
filename(filename), line(line), col(col) {

}

const Loc Loc::unknown = Loc("??", 0, 0);