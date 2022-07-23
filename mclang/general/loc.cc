
#include "general/loc.h"
#include <string>

Loc::Loc(unsigned int line, unsigned int col) : filename(Loc::nofile),
line(line), col(col) {

}

Loc::Loc(std::string filename, unsigned int line, unsigned int col) :
filename(filename), line(line), col(col) {

}

const Loc Loc::unknown = Loc(Loc::nofile, 0, 0);
const std::string Loc::nofile = "??";
