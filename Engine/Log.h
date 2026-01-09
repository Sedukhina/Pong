
enum LogLevel
{
    FATAL = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4
};

void Log(LogLevel level, const char* message);

#define LOG_FATAL(message) Log(FATAL, message)
#define LOG_ERROR(message) Log(ERROR, message)
#define LOG_WARNING(message) Log(WARNING, message)
#define LOG_INFO(message) Log(INFO, message)
#define LOG_DEBUG(message) Log(DEBUG, message)    
