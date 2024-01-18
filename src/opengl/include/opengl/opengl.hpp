#pragma once
#ifndef ENGINE_OPENGL_OPENGL_HPP_
#define ENGINE_OPENGL_OPENGL_HPP_

#include "camera.hpp"
#include "driver.hpp"
#include "resources/model.hpp"
#include "settings/comparison_algorithm.hpp"
#include "settings/cull_face.hpp"
#include "settings/operation.hpp"
#include "settings/pixel_data_format.hpp"
#include "settings/pixel_data_type.hpp"
#include "settings/primitive_mode.hpp"
#include "settings/rasterization_mode.hpp"
#include "settings/rendering_capability.hpp"

namespace Engine
{
namespace OpenGL
{
    struct FrameInfo
    {
        uint64_t BatchCount    = 0;
        uint64_t InstanceCount = 0;
        uint64_t PolyCount     = 0;
    };

    class OpenGL
    {
    public:
        OpenGL(std::shared_ptr<Driver> driver) : m_driver(driver) { }
        ~OpenGL() = default;

        OpenGL(const OpenGL& other)             = delete;
        OpenGL(OpenGL&& other)                  = delete;
        OpenGL& operator=(const OpenGL& other)  = delete;
        OpenGL& operator=(const OpenGL&& other) = delete;

        void SetClearColor(float red, float green, float blue, float alpha = 1.0f);
        void Clear(bool colorBuffer = true, bool depthBuffer = true, bool stencilBuffer = true);
        void Clear(Camera& camera, bool colorBuffer = true, bool depthBuffer = true, bool stencilBuffer = true);
        void SetRasterizationLinesWidth(float width);
        void SetRasterizationMode(Settings::ERasterizationMode rasterizationMode);
        void SetCapability(Settings::ERenderingCapability capability, bool value);
        bool GetCapability(Settings::ERenderingCapability capability) const;
        void SetStencilAlgorithm(Settings::EComparaisonAlgorithm algorithm, int32_t reference, uint32_t mask);
        void SetDepthAlgorithm(Settings::EComparaisonAlgorithm algorithm);
        void SetStencilMask(uint32_t mask);
        void SetStencilOperations(Settings::EOperation stencilFail = Settings::EOperation::Keep, Settings::EOperation depthFail = Settings::EOperation::Keep, Settings::EOperation bothPass = Settings::EOperation::Keep);
        void SetCullFace(Settings::ECullFace cullFace);
        void SetDepthWriting(bool enable);
        void SetColorWriting(bool enableRed, bool enableGreen, bool enableBlue, bool enableAlpha);
        void SetColorWriting(bool enable);
        void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Settings::EPixelDataFormat format, Settings::EPixelDataType type, void* data);
        bool GetBool(GLenum parameter);
        bool GetBool(GLenum parameter, uint32_t index);
        int GetInt(GLenum parameter);
        int GetInt(GLenum parameter, uint32_t index);
        float GetFloat(GLenum parameter);
        float GetFloat(GLenum parameter, uint32_t index);
        double GetDouble(GLenum parameter);
        double GetDouble(GLenum parameter, uint32_t index);
        int64_t GetInt64(GLenum parameter);
        int64_t GetInt64(GLenum parameter, uint32_t index);
        std::string GetString(GLenum parameter);
        std::string GetString(GLenum parameter, uint32_t index);
        void ClearFrameInfo();
        void Draw(std::shared_ptr<Resources::IMesh> mesh, Settings::EPrimitiveMode primitiveMode = Settings::EPrimitiveMode::Triangles, uint32_t instances = 1);

        uint8_t FetchGLState();
        void ApplyStateMask(uint8_t mask);
        void SetState(uint8_t state);
        const FrameInfo& GetFrameInfo() const;

    private:
        std::shared_ptr<Driver> m_driver;

        FrameInfo m_frameInfo;
        uint8_t   m_state = 0;
    };
}
}

#endif // ENGINE_OPENGL_OPENGL_HPP_
