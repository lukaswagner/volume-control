#include "helper.hpp"

#include <regex>

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
    auto replaced = replaceEnvVars(str);
    std::regex path(R"((C:)?\\.*\\\w+\.\w+)");
    std::smatch match;
    if (std::regex_search(replaced, match, path)) return match.str();
    return "";
}
