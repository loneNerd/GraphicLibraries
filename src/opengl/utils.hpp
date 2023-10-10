#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_UTILS_H_
#define GRAPHIC_LIBRARIES_OPENGL_UTILS_H_

#include <codecvt>
#include <fstream>
#include <locale>
#include <stdexcept>
#include <string>

#include <shobjidl.h>
#include <windows.h>

namespace GraphicLibraries
{
namespace OpenGL
{
    enum class EEngines
    {
        DirectX11,
        DirectX12,
        OpenGL,
        Vulkan
    };

    //struct Vertex
    //{
    //    float position[3];
    //    float color[4];
    //};

    static std::wstring getFileFullPath(const wchar_t* name)
    {
        wchar_t moduleName[_MAX_PATH] = { 0 };
        if (!GetModuleFileNameW(nullptr, moduleName, _MAX_PATH))
            throw std::runtime_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()).message().c_str());

        wchar_t drive[_MAX_DRIVE] = { 0 };
        wchar_t path[_MAX_PATH] = { 0 };

        if (_wsplitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
            throw std::runtime_error("Can't split path");

        wchar_t filename[_MAX_PATH];
        if (_wmakepath_s(filename, _MAX_PATH, drive, path, name, nullptr))
            throw std::runtime_error("Can't find filename");

        return filename;
    }

    static std::string getFileFullPath(const char* name)
    {
        char moduleName[_MAX_PATH] = { 0 };
        if (!GetModuleFileNameA(nullptr, moduleName, _MAX_PATH))
            throw std::runtime_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()).message().c_str());

        char drive[_MAX_DRIVE] = { 0 };
        char path[_MAX_PATH] = { 0 };

        if (_splitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
            throw std::runtime_error("Can't split path");

        char filename[_MAX_PATH];
        if (_makepath_s(filename, _MAX_PATH, drive, path, name, nullptr))
            throw std::runtime_error("Can't find filename");

        return filename;
    }

    static std::wstring openFileW()
    {
        OPENFILENAMEW ofn;
        WCHAR szFile[_MAX_PATH] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = _MAX_PATH;
        ofn.lpstrFilter = L"All (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameW(&ofn) == TRUE)
            return std::wstring(ofn.lpstrFile);

        return std::wstring();
    }

    static std::string openFile()
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(openFileW());
    }
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_UTILS_H_
