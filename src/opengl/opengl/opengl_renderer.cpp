#include "opengl_renderer.hpp"

namespace OGL = Engine::OpenGL;

void OGL::OpenGLRenderer::SetClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}

void OGL::OpenGLRenderer::Clear(bool colorBuffer, bool depthBuffer, bool stencilBuffer)
{
    glClear
    (
        (colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
        (depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
        (stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
    );
}

void OGL::OpenGLRenderer::Clear(Camera& camera, bool colorBuffer, bool depthBuffer, bool stencilBuffer)
{
    /* Backup the previous OpenGL clear color */
    GLfloat previousClearColor[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);

    SetClearColor(previousClearColor[0], previousClearColor[1], previousClearColor[2], previousClearColor[3]);
}

void OGL::OpenGLRenderer::SetRasterizationLinesWidth(float width)
{
    glLineWidth(width);
}

void OGL::OpenGLRenderer::SetRasterizationMode(Settings::ERasterizationMode rasterizationMode)
{
    glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(rasterizationMode));
}

void OGL::OpenGLRenderer::SetCapability(Settings::ERenderingCapability capability, bool value)
{
    (value ? glEnable : glDisable)(static_cast<GLenum>(capability));
}

bool OGL::OpenGLRenderer::GetCapability(Settings::ERenderingCapability capability) const
{
    return glIsEnabled(static_cast<GLenum>(capability));
}

void OGL::OpenGLRenderer::SetStencilAlgorithm(Settings::EComparaisonAlgorithm algorithm, int32_t reference, uint32_t mask)
{
    glStencilFunc(static_cast<GLenum>(algorithm), reference, mask);
}

void OGL::OpenGLRenderer::SetDepthAlgorithm(Settings::EComparaisonAlgorithm algorithm)
{
    glDepthFunc(static_cast<GLenum>(algorithm));
}

void OGL::OpenGLRenderer::SetStencilMask(uint32_t mask)
{
    glStencilMask(mask);
}

void OGL::OpenGLRenderer::SetStencilOperations(Settings::EOperation stencilFail, Settings::EOperation depthFail, Settings::EOperation bothPass)
{
    glStencilOp(static_cast<GLenum>(stencilFail), static_cast<GLenum>(depthFail), static_cast<GLenum>(bothPass));
}

void OGL::OpenGLRenderer::SetCullFace(Settings::ECullFace cullFace)
{
    glCullFace(static_cast<GLenum>(cullFace));
}

void OGL::OpenGLRenderer::SetDepthWriting(bool enable)
{
    glDepthMask(enable);
}

void OGL::OpenGLRenderer::SetColorWriting(bool enableRed, bool enableGreen, bool enableBlue, bool enableAlpha)
{
    glColorMask(enableRed, enableGreen, enableBlue, enableAlpha);
}

void OGL::OpenGLRenderer::SetColorWriting(bool enable)
{
    glColorMask(enable, enable, enable, enable);
}

void OGL::OpenGLRenderer::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void OGL::OpenGLRenderer::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Settings::EPixelDataFormat format, Settings::EPixelDataType type, void* data)
{
    glReadPixels(x, y, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
}

bool OGL::OpenGLRenderer::GetBool(GLenum parameter)
{
    GLboolean result;
    glGetBooleanv(parameter, &result);
    return static_cast<bool>(result);
}

bool OGL::OpenGLRenderer::GetBool(GLenum parameter, uint32_t index)
{
    GLboolean result;
    glGetBooleani_v(parameter, index, &result);
    return static_cast<bool>(result);
}

int OGL::OpenGLRenderer::GetInt(GLenum parameter)
{
    GLint result;
    glGetIntegerv(parameter, &result);
    return static_cast<int>(result);
}

int OGL::OpenGLRenderer::GetInt(GLenum parameter, uint32_t index)
{
    GLint result;
    glGetIntegeri_v(parameter, index, &result);
    return static_cast<int>(result);
}

float OGL::OpenGLRenderer::GetFloat(GLenum parameter)
{
    GLfloat result;
    glGetFloatv(parameter, &result);
    return static_cast<float>(result);
}

float OGL::OpenGLRenderer::GetFloat(GLenum parameter, uint32_t index)
{
    GLfloat result;
    glGetFloati_v(parameter, index, &result);
    return static_cast<float>(result);
}

double OGL::OpenGLRenderer::GetDouble(GLenum parameter)
{
    GLdouble result;
    glGetDoublev(parameter, &result);
    return static_cast<double>(result);
}

double OGL::OpenGLRenderer::GetDouble(GLenum parameter, uint32_t index)
{
    GLdouble result;
    glGetDoublei_v(parameter, index, &result);
    return static_cast<double>(result);
}

int64_t OGL::OpenGLRenderer::GetInt64(GLenum parameter)
{
    GLint64 result;
    glGetInteger64v(parameter, &result);
    return static_cast<int64_t>(result);
}

int64_t OGL::OpenGLRenderer::GetInt64(GLenum parameter, uint32_t index)
{
    GLint64 result;
    glGetInteger64i_v(parameter, index, &result);
    return static_cast<int64_t>(result);
}

std::string OGL::OpenGLRenderer::GetString(GLenum parameter)
{
    const GLubyte* result = glGetString(parameter);
    return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string OGL::OpenGLRenderer::GetString(GLenum parameter, uint32_t index)
{
    const GLubyte* result = glGetStringi(parameter, index);
    return result ? reinterpret_cast<const char*>(result) : std::string();
}

void OGL::OpenGLRenderer::ClearFrameInfo()
{
    m_frameInfo.BatchCount = 0;
    m_frameInfo.InstanceCount = 0;
    m_frameInfo.PolyCount = 0;
}

void OGL::OpenGLRenderer::Draw(std::shared_ptr<Resources::IMesh> mesh, Settings::EPrimitiveMode primitiveMode, uint32_t instances)
{
    if (instances > 0)
    {
        ++m_frameInfo.BatchCount;
        m_frameInfo.InstanceCount += instances;
        m_frameInfo.PolyCount += (mesh->GetIndexCount() / 3) * instances;

        mesh->Bind();

        if (mesh->GetIndexCount() > 0)
        {
            /* With EBO */
            if (instances == 1)
                glDrawElements(static_cast<GLenum>(primitiveMode), mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
            else
                glDrawElementsInstanced(static_cast<GLenum>(primitiveMode), mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr, instances);
        }
        else
        {
            /* Without EBO */
            if (instances == 1)
                glDrawArrays(static_cast<GLenum>(primitiveMode), 0, mesh->GetVertexCount());
            else
                glDrawArraysInstanced(static_cast<GLenum>(primitiveMode), 0, mesh->GetVertexCount(), instances);
        }

        mesh->Unbind();
    }
}

uint8_t OGL::OpenGLRenderer::FetchGLState()
{
    uint8_t result = 0;

    GLboolean cMask[4];
    glGetBooleanv(GL_COLOR_WRITEMASK, cMask);

    if (GetBool(GL_DEPTH_WRITEMASK))                              result |= 0b0000'0001;
    if (cMask[0])                                                 result |= 0b0000'0010;
    if (GetCapability(Settings::ERenderingCapability::Blend))     result |= 0b0000'0100;
    if (GetCapability(Settings::ERenderingCapability::CullFace))  result |= 0b0000'1000;
    if (GetCapability(Settings::ERenderingCapability::DepthTest)) result |= 0b0001'0000;

    switch (static_cast<Settings::ECullFace>(GetInt(GL_CULL_FACE)))
    {
        case Settings::ECullFace::Back:         result |= 0b0010'0000; break;
        case Settings::ECullFace::Front:        result |= 0b0100'0000; break;
        case Settings::ECullFace::FrontAndBack: result |= 0b0110'0000; break;
    }

    return result;
}

void OGL::OpenGLRenderer::ApplyStateMask(uint8_t mask)
{
    if (mask != m_state)
    {
        if ((mask & 0x01) != (m_state & 0x01)) SetDepthWriting(mask & 0x01);
        if ((mask & 0x02) != (m_state & 0x02)) SetColorWriting(mask & 0x02);
        if ((mask & 0x04) != (m_state & 0x04)) SetCapability(Settings::ERenderingCapability::Blend,     mask & 0x04);
        if ((mask & 0x08) != (m_state & 0x08)) SetCapability(Settings::ERenderingCapability::CullFace,  mask & 0x8);
        if ((mask & 0x10) != (m_state & 0x10)) SetCapability(Settings::ERenderingCapability::DepthTest, mask & 0x10);

        if ((mask & 0x08) && ((mask & 0x20) != (m_state & 0x20) || (mask & 0x40) != (m_state & 0x40)))
        {
            int backBit = mask & 0x20;
            int frontBit = mask & 0x40;
            SetCullFace(backBit && frontBit ? Settings::ECullFace::FrontAndBack : (backBit ? Settings::ECullFace::Back : Settings::ECullFace::Front));
        }

        m_state = mask;
    }
}

void OGL::OpenGLRenderer::SetState(uint8_t state)
{
    m_state = state;
}

const OGL::FrameInfo& OGL::OpenGLRenderer::GetFrameInfo() const
{
    return m_frameInfo;
}
