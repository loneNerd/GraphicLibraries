#pragma once
#ifndef ENGINE_LOGS_LOG_HANDLER_HPP_
#define ENGINE_LOGS_LOG_HANDLER_HPP_

#include <string>

namespace Engine
{
namespace Logs
{
    enum class ELogLevel
    {
        Default,
        Info,
        Warning,
        Error
    };

    enum class ELogMode
    {
        Default,
        Console
    };

    struct LogData
    {
        std::string Message;
        ELogLevel LogLevel;
        std::string Date;
    };

    class ILogHandler
    {
        friend class Logger;

    private:
        virtual void Log(const LogData& logData) = 0;
    };
}
}

#endif // ENGINE_LOGS_LOG_HANDLER_HPP_
