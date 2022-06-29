/**
 * @warning This script is only to be executed by someone with exprience with
 * programming, execution of this script may be dangerous on some systems!
 */
#include <iostream>
#include <filesystem>
#include <string>

int main () {
    unsigned int success = 0, failed = 0;
    std::string out = "";
    for (auto const &entry : std::filesystem::directory_iterator("examples")) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            if (path.find("invalid") != std::string::npos)
                continue;
#ifdef _WIN32
            if (system(("build\\main.exe -D " + path).c_str()) == 0) {
#else
            if (system(("build/main -D " + path).c_str()) == 0) {
#endif
                out += "\033[0;32mSUCCESS:    \033[0m" + path + "\n";
                success++;
            } else {
                out += "\033[0;31mFAILURE:    \033[0m" + path + "\n";
                failed++;
            }
        }
    }
    std::cout << std::endl << out << std::endl;
    if (failed > 0) {
        std::cout << "\033[0;31m" << failed << " failures, " << success
        << " successes\n\033[0m" << std::endl;
        return 1;
    } else {
        std::cout << failed << " failures, " << success << " successes"
        << std::endl;
    }
    return 0;
}