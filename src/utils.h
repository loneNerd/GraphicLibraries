#pragma once
#ifndef GRAPHICLIBRARIES_UTILS_H_
#define GRAPHICLIBRARIES_UTILS_H_

#include <exception>
#include <fstream>
#include <string>
#include <windows.h>

namespace GraphicLibraries
{
    enum class EEngines
    {
        DirectX11,
        DirectX12,
        OpenGL,
        Vulkan
    };

    struct Vertex
    {
        float position[3];
        float color[4];
    };

    static std::wstring getFileFullPath(const wchar_t* name)
    {
        wchar_t moduleName[_MAX_PATH] = { 0 };
        if (!GetModuleFileNameW(nullptr, moduleName, _MAX_PATH))
            throw std::exception(std::error_code(static_cast<int>(GetLastError()), std::system_category()).message().c_str());

        wchar_t drive[_MAX_DRIVE] = { 0 };
        wchar_t path[_MAX_PATH] = { 0 };

        if (_wsplitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
            throw std::exception("Can't split path");

        wchar_t filename[_MAX_PATH];
        if (_wmakepath_s(filename, _MAX_PATH, drive, path, name, nullptr))
            throw std::exception("Can't find filename");

        return filename;
    }
}

#endif // GRAPHICLIBRARIES_UTILS_H_
