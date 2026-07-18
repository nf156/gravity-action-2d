#include "System/config.h"
#include "System/fileSystem.h"

#include <sstream>
#include <algorithm>
#include <cctype>

std::unordered_map<std::string, std::string> Config::s_values;

bool Config::Load(const std::string& path)
{
    Clear();

    std::string text;
    if (!FileSystem::ReadAllText(path, text))
        return false;

    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line))
    {
        line = Trim(line);
        if (line.empty()) continue;

        // ƒRƒƒ“ƒgs: # ... ‚Ü‚½‚Í // ...
        if (line[0] == '#') continue;
        if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;

        const size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = Trim(line.substr(0, eqPos));
        std::string val = Trim(line.substr(eqPos + 1));

        if (key.empty()) continue;
        s_values[key] = val;
    }

    return true;
}

std::string Config::GetString(const std::string& key, const std::string& defaultValue)
{
    auto it = s_values.find(key);
    if (it == s_values.end()) return defaultValue;
    return it->second;
}

int Config::GetInt(const std::string& key, int defaultValue)
{
    auto it = s_values.find(key);
    if (it == s_values.end()) return defaultValue;

    try
    {
        return std::stoi(it->second);
    }
    catch (...)
    {
        return defaultValue;
    }
}

float Config::GetFloat(const std::string& key, float defaultValue)
{
    auto it = s_values.find(key);
    if (it == s_values.end()) return defaultValue;

    try
    {
        return std::stof(it->second);
    }
    catch (...)
    {
        return defaultValue;
    }
}

bool Config::GetBool(const std::string& key, bool defaultValue)
{
    auto it = s_values.find(key);
    if (it == s_values.end()) return defaultValue;

    std::string v = ToLower(Trim(it->second));
    if (v == "true" || v == "1" || v == "yes" || v == "on")  return true;
    if (v == "false" || v == "0" || v == "no" || v == "off") return false;
    return defaultValue;
}

bool Config::HasKey(const std::string& key)
{
    return s_values.find(key) != s_values.end();
}

void Config::Clear()
{
    s_values.clear();
}

std::string Config::Trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;

    return s.substr(start, end - start);
}

std::string Config::ToLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}