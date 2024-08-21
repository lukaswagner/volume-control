#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include <combaseapi.h>

namespace VolumeControl
{
std::string toString(LPWSTR wstr, boolean freeWStr = true);
std::string replaceEnvVars(std::string str);
std::string getPath(std::string str);
std::string toDosPath(std::string path);
std::string guidToString(GUID& guid);

#define CHECK(status, message)                                                 \
    if (status != S_OK) throw std::runtime_error(message);

#define DEBUG_VAL(value) std::cerr << #value << ": " << (value) << std::endl;

#define PADDED_HEADER(stream, text, width)                                     \
    stream << text << std::string(width - strlen(text), ' ') << ": "
#define DUMPTO(stream, text) PADDED_HEADER(stream, text, 15)
#define DUMP(text) DUMPTO(stream, text)

#define SAFE_RELEASE(p) if ((p)) { (p)->Release(); (p) = 0; }
}
