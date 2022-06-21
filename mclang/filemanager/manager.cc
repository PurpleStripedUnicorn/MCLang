
#include "filemanager/manager.h"

FileManager::FileManager(std::string root, std::string ns) : root(root), ns(ns)
{
    deletePrevPack();
    createSubFolder("");
}

FileManager::~FileManager() {

}

void FileManager::genDatapack(const std::vector<CmdFunc> &cmds) const {
    genFolderStructure();
    genPackFile();
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

void FileManager::genPackFile() const {
    std::ofstream file(root + DIRSEP + "pack.mcmeta");
    if (file.is_open()) {
        file << "{\"pack\":{\"pack_format\":10,"
        << "\"description\":\"Generated with MCLang.\"}}";
    } else {
        MCLError(1, "Could not write to file 'pack.mcmeta'.", 0, 0);
    }
}

void FileManager::deletePrevPack() const {
    if (!std::filesystem::is_directory(root))
        return;
    std::ifstream metaFile(root + DIRSEP + "pack.mcmeta");
    if (metaFile.good() || std::filesystem::is_empty(root))
        std::filesystem::remove_all(root);
    else
        MCLError(1, "Output folder exists, but is not a datapack.", 0, 0);
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