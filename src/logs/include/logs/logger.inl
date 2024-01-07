#include "logger.hpp"

namespace Logs = Engine::Logs;

template<typename T>
static inline void Logs::Logger::logToHandlerMap(std::map<std::string, T>& map, const LogData& data, std::string id)
{
    if (id != "default")
        map[id].Log(data);
    else
    {
        if (map.find("default") != map.end())
            map["default"].Log(data);
        else
        {
            map.emplace("default", T());
            map["default"].Log(data);
        }
    }
}
