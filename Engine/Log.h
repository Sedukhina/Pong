#pragma once

enum class LogLevel
{
    FATAL = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4
};

void Log(LogLevel Level, const char* Message);

#define LOG_FATAL(message) Log(LogLevel::FATAL, message)
#define LOG_ERROR(message) Log(LogLevel::ERROR, message)
#define LOG_WARNING(message) Log(LogLevel::WARNING, message)
#define LOG_INFO(message) Log(LogLevel::INFO, message)
#define LOG_DEBUG(message) Log(LogLevel::DEBUG, message)    
