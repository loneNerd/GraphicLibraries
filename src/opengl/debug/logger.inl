#include "logger.hpp"

namespace Debug = Engine::Debug;

template<typename T>
static inline void Debug::Logger::logToHandlerMap(std::map<std::string, T>& map, const LogData& data, std::string id)
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
