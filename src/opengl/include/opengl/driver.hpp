#pragma once
#ifndef ENGINE_OPENGL_DRIVER_HPP_
#define ENGINE_OPENGL_DRIVER_HPP_

#include <stdint.h>

namespace Engine
{
namespace OpenGL
{
    class Driver
    {
    public:
        Driver(bool debug = false);
        ~Driver() = default;

        Driver(const Driver& other)             = delete;
        Driver(Driver&& other)                  = delete;
        Driver& operator=(const Driver& other)  = delete;
        Driver& operator=(const Driver&& other) = delete;

        bool IsActive() const { return m_isActive; }

    private:
        static void __stdcall GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam);
        void initGlew();

        bool m_isActive = false;

    };
}
}

#endif // ENGINE_OPENGL_DRIVER_HPP_
