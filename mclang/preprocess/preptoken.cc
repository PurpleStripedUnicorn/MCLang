
#include "preprocess/preptoken.h"
#include <string>

PrepToken::PrepToken(PrepTokenType type) : type(type), content("") {

}

PrepToken::PrepToken(PrepTokenType type, std::string content) : type(type),
content(content) {

}