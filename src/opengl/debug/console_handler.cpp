#include "console_handler.hpp"

#include "console_color.hpp"

namespace Debug = Engine::Debug;

std::string const Debug::ConsoleHandler::DEFAULT_HEADER = "";
std::string const Debug::ConsoleHandler::INFO_HEADER    = "[INFO] ";
std::string const Debug::ConsoleHandler::WARNING_HEADER = "[WARNING] ";
std::string const Debug::ConsoleHandler::ERROR_HEADER   = "[ERROR] ";

void Debug::ConsoleHandler::Log(const LogData& logData)
{
    switch (logData.LogLevel)
    {
        case ELogLevel::Default:
        {
            std::cout << COLOR_WHITE;
            break;
        }
        case ELogLevel::Info:
        {
            std::cout << COLOR_BLUE;
            break;
        }
        case ELogLevel::Warning:
        {
            std::cout << COLOR_YELLOW;
            break;
        }
        case ELogLevel::Error:
        {
            std::cout << COLOR_RED;
            break;
        }
    }

    std::ostream& output = logData.LogLevel == ELogLevel::Error ? std::cerr : std::cout;

    output << getLogHeader(logData.LogLevel) << logData.Date << " " << logData.Message << std::endl;

    std::cout << COLOR_DEFAULT;
}

std::string Debug::ConsoleHandler::getLogHeader(ELogLevel logLevel)
{
    switch (logLevel)
    {
        case ELogLevel::Default: return DEFAULT_HEADER;
        case ELogLevel::Info:    return INFO_HEADER;
        case ELogLevel::Warning: return WARNING_HEADER;
        case ELogLevel::Error:   return ERROR_HEADER;
    }

    return "";
}
