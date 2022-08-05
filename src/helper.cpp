#include "helper.hpp"

#include <regex>

#include <fileapi.h>

std::string toString(LPWSTR wstr)
{
    auto len = wcstombs(NULL, wstr, 0);
    if (len == 0) return "";
    auto cstr = (char*)malloc((len + 1) * sizeof(char));
    len = wcstombs(cstr, wstr, len + 1);
    auto result = std::string(cstr);
    free(cstr);
    CoTaskMemFree(wstr);
    return result;
}

std::string replaceEnvVars(std::string str)
{
    auto result = str.substr();
    auto start = result.cbegin();
    std::smatch match;
    std::regex env("%[^%]*%");
    while (std::regex_search(start, result.cend(), match, env))
    {
        auto m = match[0];
        auto envName = m.str().substr(1, m.length() - 2);
        auto replacement = std::getenv(envName.c_str());
        if (replacement == nullptr)
        {
            start = m.second;
        }
        else
        {
            result.replace(m.first, m.second, replacement);
            start = m.first + strlen(replacement);
        }
    }
    return result;
}

std::string getPath(std::string str)
{
    std::string result = "";
    auto replaced = replaceEnvVars(str);
    std::regex path(R"((C:)?\\.*\\\w+\.\w+)");
    std::smatch match;
    if (std::regex_search(replaced, match, path)) result = match.str();
    if (result.at(0) == '\\') result = toDosPath(result);
    return result;
}

std::map<std::string, std::string> deviceMap;
void buildDeviceMap()
{
    deviceMap.clear();

    std::string inStr;
    std::string outStr;

    std::wstring inWideStr;
    std::wstring outWideStr;

    const size_t inChars = 256;
    size_t outChars;

    wchar_t outWideCStr[inChars];

    for (char c = 'C'; c <= 'Z'; ++c)
    {
        inStr = std::string(1, c).append(":");
        inWideStr = std::wstring(inStr.begin(), inStr.end());
        outChars = QueryDosDeviceW(inWideStr.c_str(), outWideCStr, inChars);
        if (outChars == 0)
        {
            auto err = GetLastError();
            continue;
        }
        outWideStr = std::wstring(outWideCStr);
        if (outWideStr.length() == 0) continue;
        outStr = std::string(outWideStr.begin(), outWideStr.end());
        deviceMap[outStr] = inStr;
    }

#if false
    std::cerr << "### device map ###" << std::endl;
    for (auto&& p : deviceMap)
    {
        std::cerr << p.first << " -> " << p.second << std::endl;
    }
#endif
}

std::string toDosPath(std::string path)
{
    if (deviceMap.size() == 0) buildDeviceMap();
    for (auto&& p : deviceMap)
    {
        auto start = path.find(p.first);
        if (start != -1)
            path.replace(start, start + p.first.length(), p.second);
    }
    return path;
}
