#pragma once
#ifndef ENGINE_LOGS_LOGGER_HPP_
#define ENGINE_LOGS_LOGGER_HPP_

#include <string>
#include <map>

#include "console_handler.hpp"
#include "log_handler.hpp"

#define LOG(message)         Engine::Logs::Logger::Log(message, Engine::Logs::ELogLevel::Default, Engine::Logs::ELogMode::Console)
#define LOG_INFO(message)    Engine::Logs::Logger::Log(message, Engine::Logs::ELogLevel::Info,    Engine::Logs::ELogMode::Console)
#define LOG_WARNING(message) Engine::Logs::Logger::Log(message, Engine::Logs::ELogLevel::Warning, Engine::Logs::ELogMode::Console)
#define LOG_ERROR(message)   Engine::Logs::Logger::Log(message, Engine::Logs::ELogLevel::Error,   Engine::Logs::ELogMode::Console)

namespace Engine::Logs
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

#include "logger.inl"

#endif // ENGINE_LOGS_LOGGER_HPP_
