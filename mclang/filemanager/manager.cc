
#include "bcconvert/bcconvert.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "filemanager/manager.h"
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef OS_WINDOWS
    #include <windows.h>
    #define MKDIR_FAIL_CODE -1
    #define DIRSEP std::string("\\")
#else
    #define MKDIR_FAIL_CODE -1
    #define DIRSEP std::string("/")
#endif

FileManager::FileManager(Compiler *comp) : root(comp->outputFolder),
ns(comp->ns), comp(comp) {
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
        MCLError(1, "Could not write to file '" + func.name + ".mcfunction'.");
    }
    file.close();
}

int FileManager::getPackFormat() const {
    std::string ver = comp->mcVersion;
    if (packVersions.find(ver) == packVersions.end())
        MCLError(1, "Unsupported version given: '" + ver + "'.");
    return packVersions.find(ver)->second;
}

void FileManager::genPackFile() const {
    std::string description = comp->description;
    if (description == "")
        description = "Generated with MCLang, see "
        "https://github.com/PurpleStripedUnicorn/MCLang";
    std::ofstream file(root + DIRSEP + "pack.mcmeta");
    if (file.is_open()) {
        file << "{\"pack\":{\"pack_format\":" << getPackFormat() << ","
        << "\"description\":\"" << description << "\"}}";
    } else {
        MCLError(1, "Could not write to file 'pack.mcmeta'.");
    }
}

void FileManager::deletePrevPack() const {
    if (!std::filesystem::is_directory(root))
        return;
    std::ifstream metaFile(root + DIRSEP + "pack.mcmeta");
    if (metaFile.good() || std::filesystem::is_empty(root)) {
        metaFile.close();
        std::error_code ec;
        std::filesystem::remove_all(root, ec);
        if (ec.value() != 0)
            MCLError(1, "Something went wrong while deleting old directory.");
    } else {
        metaFile.close();
        MCLError(1, "Output folder exists, but is not a datapack.");
    }
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
#ifdef OS_WINDOWS
    int check = mkdir(path.c_str());
#else
    int check = mkdir(path.c_str(), 0777);
#endif
    if (check == MKDIR_FAIL_CODE) {
        MCLError(1, "Could not create folder '" + path + "'");
    }
}