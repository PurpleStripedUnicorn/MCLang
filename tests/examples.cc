/**
 * @warning This script is only to be executed by someone with exprience with
 * programming, execution of this script may be dangerous on some systems!
 */
#include <iostream>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#ifdef _WIN32
    #define DIRSEP "\\"
#else
    #define DIRSEP "/"
#endif

std::set<std::string> getFileNames(std::string path = "examples") {
    std::set<std::string> out;
    for (auto const &entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file())
            out.insert(entry.path().string());
        if (entry.is_directory()) {
            std::set<std::string> sub = getFileNames(entry.path().string());
            out.insert(sub.begin(), sub.end());
        }
    }
    return out;
}

void printFileResult(std::string name, bool hasFailed) {
    std::cout << (hasFailed ? "\033[0;31mFAILURE:" : "\033[0;32mSUCCESS:")
    << "    \033[0m" << name << std::endl;
}

int main () {
    unsigned int success = 0, failed = 0;
    std::vector<std::string> failedList;
    std::cout << std::endl;
    for (const std::string &path : getFileNames()) {
        if (path.find("invalid") != std::string::npos)
            continue;
#ifdef _WIN32
        if (system(("build\\main.exe -D " + path).c_str()) == 0) {
#else
        if (system(("build/main -D " + path).c_str()) == 0) {
#endif
            printFileResult(path, false);
            success++;
        } else {
            printFileResult(path, true);
            failedList.push_back(path);
            failed++;
        }
    }
    std::cout << std::endl << (failed > 0 ? "\033[0;31m" : "\033[0;32m")
    << failed << " failures, " << success << " successes\033[0m" << std::endl
    << std::endl;
    if (failed > 0) {
        std::cout << "\033[0;31mList of failures:" << std::endl;
        for (unsigned int i = 0; i < failedList.size(); i++)
            std::cout << "    " << failedList[i] << std::endl;
        std::cout << "\033[0m" << std::endl;
    }
    return failed > 0 ? 1 : 0;
}