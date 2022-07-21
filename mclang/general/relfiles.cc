
#include "general/relfiles.h"
#include <filesystem>

std::string getReferencePath(std::string base, std::string ref) {
    std::filesystem::path refPath = std::filesystem::path(ref).make_preferred();
    std::filesystem::path basePath = std::filesystem::path(base);
    std::filesystem::path baseFolder = basePath.parent_path();
    return (baseFolder / refPath).string();
}