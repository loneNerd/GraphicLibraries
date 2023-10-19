#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_WIDGETS_SETTINGS_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_WIDGETS_SETTINGS_HPP_

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Widgets
{
    class Settings
    {
    public:
        Settings() { }
        ~Settings() { }

        Settings(const Settings& other)            = delete;
        Settings(Settings&& other)                 = delete;
        Settings& operator=(const Settings& other) = delete;
        Settings& operator=(Settings&& other)      = delete;

        void draw();
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_WIDGETS_SETTINGS_HPP_
