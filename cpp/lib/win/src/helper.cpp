#include "helper.hpp"

#include <regex>

#include <fileapi.h>

namespace VolumeControl
{
auto locale = _create_locale(LC_ALL, ".utf-8");
std::string toString(LPWSTR wstr, boolean freeWStr)
{
    size_t len;
    auto result = _wcstombs_s_l(&len, NULL, 0, wstr, 0, locale);
    if (result != S_OK) throw std::runtime_error("could not determine string length");
    if (len == 0) return "";

    auto cstr = (char*)malloc((len + 1) * sizeof(char));
    result = _wcstombs_s_l(&len, cstr, len + 1, wstr, len, locale);
    if (result != S_OK) throw std::runtime_error("could not convert string");

    auto string = std::string(cstr);
    free(cstr);
    if(freeWStr) CoTaskMemFree(wstr);
    return string;
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

        // ignore warning for stdlib call
        #pragma warning(disable: 4996)
        auto replacement = std::getenv(envName.c_str());
        #pragma warning(default: 4996)

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
        outStr = toString(outWideCStr, false);
        if (outStr.length() == 0) continue;
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
}
