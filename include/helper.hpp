#pragma once

#include <string>

std::string toString(LPWSTR wstr)
{
    auto len = wcstombs(NULL, wstr, 0);
    auto cstr = (char*)malloc(len * sizeof(char));
    len = wcstombs(cstr, wstr, len);
    auto result = std::string(cstr);
    free(cstr);
    CoTaskMemFree(wstr);
    return result;
}
