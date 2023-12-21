#include "logger.hpp"

#include "tools/time/date.hpp"

namespace Debug = Engine::Debug;
namespace Time = Engine::Tools::Time;

std::map<std::string, Debug::ConsoleHandler> Debug::Logger::ConsoleHandlerMap;

void Debug::Logger::Log(const std::string& message, ELogLevel logLevel, ELogMode logMode, std::string handlerId)
{
    LogData logData { message, logLevel, Time::Date::GetDateAsString() };

    switch (logMode)
    {
        case ELogMode::Default:
        case ELogMode::Console: logToHandlerMap<ConsoleHandler>(ConsoleHandlerMap, logData, handlerId); break;
    }
}

Debug::ConsoleHandler& Debug::Logger::CreateConsoleHandler(std::string id)
{
    ConsoleHandlerMap.emplace(id, ConsoleHandler());
    return ConsoleHandlerMap[id];
}

Debug::ConsoleHandler& Debug::Logger::GetConsoleHandler(std::string id)
{
    return ConsoleHandlerMap[id];
}
