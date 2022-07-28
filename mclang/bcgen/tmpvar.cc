
#include "bcgen/tmpvar.h"
#include <set>
#include <string>

TmpVarManager::TmpVarManager() {

}

TmpVarManager::~TmpVarManager() {

}

std::string TmpVarManager::getFree() const {
    return getVarName(firstFreeIndex());
}

std::string TmpVarManager::reserve() {
    unsigned int index = firstFreeIndex();
    reserve(index);
    return getVarName(index);
}

void TmpVarManager::reserve(std::string name) {
    reserve(getVarIndex(name));
}

void TmpVarManager::free(std::string name) {
    free(getVarIndex(name));
}

std::set<std::string> TmpVarManager::getReserved() const {
    std::set<std::string> out;
    for (const unsigned int &index : reserved)
        out.insert(getVarName(index));
    return out;
}

void TmpVarManager::clear() {
    reserved.clear();
}

void TmpVarManager::reserve(unsigned int index) {
    if (reserved.count(index) == 0)
        reserved.insert(index);
}

void TmpVarManager::free(unsigned int index) {
    if (reserved.count(index) > 0)
        reserved.erase(index);
}

std::string TmpVarManager::getVarName(unsigned int index) const {
    return prefix() + std::to_string(index);
}

unsigned int TmpVarManager::getVarIndex(std::string name) const {
    return std::stoi(name.substr(prefix().size()));
}

std::string TmpVarManager::prefix() const {
    return "0tmp";
}

unsigned int TmpVarManager::firstFreeIndex() const {
    unsigned int i = 0;
    while (reserved.count(i) > 0)
        i++;
    return i;
}