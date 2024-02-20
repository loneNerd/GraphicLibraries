#include "ui/panels/panel_transformable.hpp"
#include "ui/internal/converter.hpp"

#include <imgui/imgui.h>

namespace Internal = Engine::UI::Internal;
namespace Math = Engine::Math;
namespace Panels = Engine::UI::Panels;
namespace Settings = Engine::UI::Settings;

void Panels::APanelTransformable::SetPosition(const Math::FVector2& position)
{
    m_position = position;
    m_positionChanged = true;
}

void Panels::APanelTransformable::SetSize(const Math::FVector2& size)
{
    m_size = size;
    m_sizeChanged = true;
}

void Panels::APanelTransformable::SetAlignment(Settings::EHorizontalAlignment horizontalAlignment, Settings::EVerticalAlignment verticalAligment)
{
    m_horizontalAlignment = horizontalAlignment;
    m_verticalAlignment = verticalAligment;
    m_alignmentChanged = true;
}

const Math::FVector2& Panels::APanelTransformable::GetPosition() const
{
    return m_position;
}

const Math::FVector2& Panels::APanelTransformable::GetSize() const
{
    return m_size;
}

Settings::EHorizontalAlignment Panels::APanelTransformable::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

Settings::EVerticalAlignment Panels::APanelTransformable::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}

void Panels::APanelTransformable::updatePosition()
{
    if (m_defaultPosition.X != -1.0f && m_defaultPosition.Y != 1.0f)
    {
        Math::FVector2 offsettedDefaultPos = m_defaultPosition + calculatePositionAlignmentOffset(true);
        ImGui::SetWindowPos(Internal::Converter::ToImVec2(offsettedDefaultPos), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
    }

    if (m_positionChanged || m_alignmentChanged)
    {
        Math::FVector2 offset = calculatePositionAlignmentOffset(false);
        Math::FVector2 offsettedPos(m_position.X + offset.X, m_position.Y + offset.Y);
        ImGui::SetWindowPos(Internal::Converter::ToImVec2(offsettedPos), ImGuiCond_Always);
        m_positionChanged = false;
        m_alignmentChanged = false;
    }
}

void Panels::APanelTransformable::updateSize()
{
    if (m_sizeChanged)
    {
        ImGui::SetWindowSize(Internal::Converter::ToImVec2(m_size), ImGuiCond_Always);
        m_sizeChanged = false;
    }
}

void Panels::APanelTransformable::copyImGuiPosition()
{
    m_position = Internal::Converter::ToFVector2(ImGui::GetWindowPos());
}

void Panels::APanelTransformable::copyImGuiSize()
{
    m_size = Internal::Converter::ToFVector2(ImGui::GetWindowSize());
}

void Panels::APanelTransformable::Update()
{
    if (!m_firstFrame)
    {
        if (!m_autoSize)
            updateSize();
        copyImGuiSize();

        updatePosition();
        copyImGuiPosition();
    }

    m_firstFrame = false;
}

Math::FVector2 Panels::APanelTransformable::calculatePositionAlignmentOffset(bool default)
{
    Math::FVector2 result(0.0f, 0.0f);

    switch (default ? m_defaultHorizontalAlignment : m_horizontalAlignment)
    {
    case Settings::EHorizontalAlignment::Center:
        result.X -= m_size.X / 2.0f;
        break;
    case Settings::EHorizontalAlignment::Right:
        result.X -= m_size.X;
        break;
    }

    switch (default ? m_defaultVerticalAlignment : m_verticalAlignment)
    {
    case Settings::EVerticalAlignment::Middle:
        result.Y -= m_size.Y / 2.0f;
        break;
    case Settings::EVerticalAlignment::Bottom:
        result.Y -= m_size.Y;
        break;
    }

    return result;
}
