#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include <combaseapi.h>

std::string toString(LPWSTR wstr);
std::string replaceEnvVars(std::string str);
std::string getPath(std::string str);

#define CHECK(status, message)                                                 \
    if (status != S_OK) throw std::runtime_error(message);

#define DEBUG_VAL(value) std::cerr << #value << ": " << (value) << std::endl;
