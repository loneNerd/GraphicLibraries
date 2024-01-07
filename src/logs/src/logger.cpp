#include "logger.hpp"

#include "tools/time/date.hpp"

namespace Logs = Engine::Logs;
namespace Time = Engine::Tools::Time;

std::map<std::string, Logs::ConsoleHandler> Logs::Logger::ConsoleHandlerMap;

void Logs::Logger::Log(const std::string& message, ELogLevel logLevel, ELogMode logMode, std::string handlerId)
{
    LogData logData { message, logLevel, Time::Date::GetDateAsString() };

    switch (logMode)
    {
        case ELogMode::Default:
        case ELogMode::Console: logToHandlerMap<ConsoleHandler>(ConsoleHandlerMap, logData, handlerId); break;
    }
}

Logs::ConsoleHandler& Logs::Logger::CreateConsoleHandler(std::string id)
{
    ConsoleHandlerMap.emplace(id, ConsoleHandler());
    return ConsoleHandlerMap[id];
}

Logs::ConsoleHandler& Logs::Logger::GetConsoleHandler(std::string id)
{
    return ConsoleHandlerMap[id];
}
