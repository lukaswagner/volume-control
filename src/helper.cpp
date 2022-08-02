#include "helper.hpp"

std::string toString(LPWSTR wstr)
{
    auto len = wcstombs(NULL, wstr, 0);
    if (len == 0) return "";
    auto cstr = (char*)malloc((len + 1) * sizeof(char));
    len = wcstombs(cstr, wstr, len);
    auto result = std::string(cstr);
    free(cstr);
    CoTaskMemFree(wstr);
    return result;
}
