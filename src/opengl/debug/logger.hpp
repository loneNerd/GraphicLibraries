#pragma once
#ifndef ENGINE_DEBUG_LOGGER_HPP_
#define ENGINE_DEBUG_LOGGER_HPP_

#include <string>
#include <map>

#include "console_handler.hpp"
#include "log_handler.hpp"

#define LOG(message)         Engine::Debug::Logger::Log(message, Engine::Debug::ELogLevel::Default, Engine::Debug::ELogMode::Console)
#define LOG_INFO(message)    Engine::Debug::Logger::Log(message, Engine::Debug::ELogLevel::Info,    Engine::Debug::ELogMode::Console)
#define LOG_WARNING(message) Engine::Debug::Logger::Log(message, Engine::Debug::ELogLevel::Warning, Engine::Debug::ELogMode::Console)
#define LOG_ERROR(message)   Engine::Debug::Logger::Log(message, Engine::Debug::ELogLevel::Error,   Engine::Debug::ELogMode::Console)

namespace Engine
{
namespace Debug
{
    class Logger
    {
    public:
        Logger() = delete;
        static void Log(const std::string& message, ELogLevel logLevel = ELogLevel::Default, ELogMode logMode = ELogMode::Default, std::string handlerId = "default");

        Logger(const Logger& other)             = delete;
        Logger(Logger&& other)                  = delete;
        Logger& operator=(const Logger& other)  = delete;
        Logger& operator=(const Logger&& other) = delete;

        static ConsoleHandler& CreateConsoleHandler(std::string id);
        static ConsoleHandler& GetConsoleHandler(std::string id);

    private:
        template<typename T>
        static void logToHandlerMap(std::map<std::string, T>& map, const LogData& data, std::string id);

        static std::map<std::string, ConsoleHandler> ConsoleHandlerMap;
    };
}
}

#include "logger.inl"

#endif // ENGINE_DEBUG_LOGGER_HPP_
