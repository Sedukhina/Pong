#include "Log.h"
#include <cstdio>
#include <magic_enum.hpp>

void Log(LogLevel Level, const char* Message)
{
    printf("%s: %s\n",
        std::string(magic_enum::enum_name(Level)).c_str(),
        Message);
}