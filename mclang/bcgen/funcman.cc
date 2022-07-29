
#include "bcgen/funcman.h"
#include "bcgen/bcgen.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <map>
#include <string>
#include <vector>

FuncManager::FuncManager(BCManager &bcman) : bcman(bcman) {

}

FuncManager::~FuncManager() {
    
}