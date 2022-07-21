
#include "preprocess/preptoken.h"
#include <string>

PrepToken::PrepToken(PrepTokenType type, Loc loc) : type(type), content(""),
loc(loc) {

}

PrepToken::PrepToken(PrepTokenType type, std::string content, Loc loc) :
type(type), content(content), loc(loc) {

}