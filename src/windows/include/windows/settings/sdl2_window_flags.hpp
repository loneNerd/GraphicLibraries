#pragma once
#ifndef ENGINE_WINDOWS_SETTINGS_SDL2_WINDOW_FLAGS_HPP_
#define ENGINE_WINDOWS_SETTINGS_SDL2_WINDOW_FLAGS_HPP_

#include <stdint.h>

namespace Engine::Windows::Settings
{
    enum class ESDL2WindowFlags : uint32_t
    {
        None              = 0x00000000,
        Fullscreen        = 0x00000001,
        OpenGL            = 0x00000002,
        Shown             = 0x00000004,
        Hidden            = 0x00000008,
        Borderless        = 0x00000010,
        Resizable         = 0x00000020,
        Minimized         = 0x00000040,
        Maximized         = 0x00000080,
        MouseGrabbed      = 0x00000100,
        InputFocus        = 0x00000200,
        MouseFocus        = 0x00000400,
        FullscreenDesktop = (Fullscreen | 0x00001000),
        Foreign           = 0x00000800,
        AllowHighdpi      = 0x00002000,
        MouseCapture      = 0x00004000,
        AlwaysOnTop       = 0x00008000,
        SkipTaskbar       = 0x00010000,
        Utility           = 0x00020000,
        Tooltip           = 0x00040000,
        PopupMenu         = 0x00080000,
        KeyboardGrabbed   = 0x00100000,
        Vulkan            = 0x10000000,
        Metal             = 0x20000000,
        InputGrabbed      = MouseGrabbed
    };

    inline ESDL2WindowFlags operator~   (ESDL2WindowFlags  a) { return (ESDL2WindowFlags)~(int)a; }
    inline ESDL2WindowFlags operator|   (ESDL2WindowFlags  a, ESDL2WindowFlags b) { return (ESDL2WindowFlags)((int)a | (int)b); }
    inline ESDL2WindowFlags operator&   (ESDL2WindowFlags  a, ESDL2WindowFlags b) { return (ESDL2WindowFlags)((int)a & (int)b); }
    inline ESDL2WindowFlags operator^   (ESDL2WindowFlags  a, ESDL2WindowFlags b) { return (ESDL2WindowFlags)((int)a ^ (int)b); }
    inline ESDL2WindowFlags& operator|= (ESDL2WindowFlags& a, ESDL2WindowFlags b) { return (ESDL2WindowFlags&)((int&)a |= (int)b); }
    inline ESDL2WindowFlags& operator&= (ESDL2WindowFlags& a, ESDL2WindowFlags b) { return (ESDL2WindowFlags&)((int&)a &= (int)b); }
    inline ESDL2WindowFlags& operator^= (ESDL2WindowFlags& a, ESDL2WindowFlags b) { return (ESDL2WindowFlags&)((int&)a ^= (int)b); }
}

#endif // ENGINE_WINDOWS_SETTINGS_SDL2_WINDOW_FLAGS_HPP_
