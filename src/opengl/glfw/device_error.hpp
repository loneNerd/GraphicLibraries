#pragma once
#ifndef ENGINE_GLFW_DEVICE_ERROR_HPP_
#define ENGINE_GLFW_DEVICE_ERROR_HPP_

namespace Engine
{
namespace GLFW
{
    enum class EDeviceError
    {
        NotInitialized,
        NoCurrentContext,
        InvalidEnum,
        InvalidValue,
        OutOfMemory,
        ApiUnavailable,
        VersionUnavailable,
        PlatformError,
        FormatUnavailable,
        NoWindowContext
    };
}
}

#endif // ENGINE_GLFW_DEVICE_ERROR_HPP_
