#include "System/fileSystem.h"
#include <fstream>
#include <sstream>

bool FileSystem::Exists(const std::string& path)
{
    std::ifstream ifs(path, std::ios::binary);
    return ifs.good();
}

bool FileSystem::ReadAllText(const std::string& path, std::string& outText)
{
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open())
        return false;

    std::ostringstream ss;
    ss << ifs.rdbuf();
    outText = ss.str();
    return true;
}