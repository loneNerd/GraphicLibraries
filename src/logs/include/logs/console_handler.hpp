#pragma once
#ifndef ENGINE_LOGS_CONSOLE_HANDLER_HPP_
#define ENGINE_LOGS_CONSOLE_HANDLER_HPP_

#include "log_handler.hpp"

namespace Engine::Logs
{
    class ConsoleHandler : public ILogHandler
    {
    public:
        void Log(const LogData& logData) override;

    private:
        static std::string getLogHeader(ELogLevel logLevel);

        static const std::string DEFAULT_HEADER;
        static const std::string INFO_HEADER;
        static const std::string WARNING_HEADER;
        static const std::string ERROR_HEADER;
    };
}

#endif // ENGINE_LOGS_CONSOLE_HANDLER_HPP_
