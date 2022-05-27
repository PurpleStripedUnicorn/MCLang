
#include "filemanager/manager.h"

FileManager::FileManager(std::string root, std::string ns) : root(root), ns(ns)
{
    // TODO: Implement deleting the root directory
    // TODO: Implement writing to pack.mcmeta
    createSubFolder("");
}

FileManager::~FileManager() {

}

void FileManager::genDatapack(const std::vector<CmdFunc> &cmds) const {
    genFolderStructure();
    for (unsigned int i = 0; i < cmds.size(); i++)
        genFunctionFile(cmds[i]);
}

void FileManager::genFunctionFile(const CmdFunc &func) const {
    std::ofstream file(root + DIRSEP + "data" + DIRSEP + ns + DIRSEP
    + "functions" + DIRSEP + func.name + ".mcfunction");
    if (file.is_open()) {
        for (unsigned int j = 0; j < func.cmdList.size(); j++)
            file << func.cmdList[j] << std::endl;
    } else {
        MCLError(1, "Could not write to file '" + func.name + ".mcfunction'.",
        0, 0);
    }
    file.close();
}

void FileManager::genFolderStructure() const {
    createSubFolder("data");
    createSubFolder("data" + DIRSEP + ns);
    createSubFolder("data" + DIRSEP + ns + DIRSEP + "functions");
}

void FileManager::createSubFolder(std::string path) const {
    createFolder(root + DIRSEP + path);
}

void FileManager::createFolder(std::string path) const {
    int check = mkdir(path.c_str(), 0777);
    if (check == MKDIR_FAIL_CODE)
        MCLError(1, "Could not create folder '" + path + "'", 0, 0);
}