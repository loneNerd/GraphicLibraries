#include "console_handler.hpp"

#include "console_color.hpp"

namespace Logs = Engine::Logs;

std::string const Logs::ConsoleHandler::DEFAULT_HEADER = "";
std::string const Logs::ConsoleHandler::INFO_HEADER    = "[INFO] ";
std::string const Logs::ConsoleHandler::WARNING_HEADER = "[WARNING] ";
std::string const Logs::ConsoleHandler::ERROR_HEADER   = "[ERROR] ";

void Logs::ConsoleHandler::Log(const LogData& logData)
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

std::string Logs::ConsoleHandler::getLogHeader(ELogLevel logLevel)
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
