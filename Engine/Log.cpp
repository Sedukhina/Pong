#include "Log.h"
#include<cstdio>
#include <magic_enum.hpp>

void Log(LogLevel level, const char* message)
{
    printf("%s: %s\n",
        std::string(magic_enum::enum_name(level)).c_str(),
        message);
}

