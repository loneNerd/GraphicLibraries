#pragma once
#ifndef ENGINE_UI_PANELS_PANEL_TRANSFORMABLE_HPP_
#define ENGINE_UI_PANELS_PANEL_TRANSFORMABLE_HPP_

#include "base_panel.hpp"
#include "ui/settings/alignment.hpp"

#include "math/vector2.hpp"

namespace Engine::UI::Panels
{
    class APanelTransformable : public BasePanel
    {
    public:
        APanelTransformable
        (
            const Math::FVector2& defaultPosition = Math::FVector2(-1.0f, -1.0f),
            const Math::FVector2& defaultSize = Math::FVector2(-1.0f, -1.0f),
            Settings::EHorizontalAlignment defaultHorizontalAlignment = Settings::EHorizontalAlignment::Left,
            Settings::EVerticalAlignment defaultVerticalAlignment = Settings::EVerticalAlignment::Top,
            bool ignoreConfigFile = false
        ) : m_defaultPosition(defaultPosition),
            m_defaultSize(defaultSize),
            m_defaultHorizontalAlignment(defaultHorizontalAlignment),
            m_defaultVerticalAlignment(defaultVerticalAlignment),
            m_ignoreConfigFile(ignoreConfigFile) { }

        APanelTransformable(const APanelTransformable& other)             = delete;
        APanelTransformable(APanelTransformable&& other)                  = delete;
        APanelTransformable& operator=(const APanelTransformable& other)  = delete;
        APanelTransformable& operator=(const APanelTransformable&& other) = delete;

        void SetPosition(const Math::FVector2& position);
        void SetSize(const Math::FVector2& size);
        void SetAlignment(Settings::EHorizontalAlignment horizontalAlignment, Settings::EVerticalAlignment verticalAligment);
        const Math::FVector2& GetPosition() const;
        const Math::FVector2& GetSize() const;
        Settings::EHorizontalAlignment GetHorizontalAlignment() const;
        Settings::EVerticalAlignment GetVerticalAlignment() const;

        bool IsAutoSize() const { return m_autoSize; }
        void SetAutoSize(bool autoSize) { m_autoSize = autoSize; }

    protected:
        void Update();
        virtual void DrawImpl() = 0;

        Math::FVector2 m_defaultPosition = Math::FVector2(-1.0f, -1.0f);
        Math::FVector2 m_defaultSize = Math::FVector2(-1.0f, -1.0f);
        Settings::EHorizontalAlignment m_defaultHorizontalAlignment = Settings::EHorizontalAlignment::Left;
        Settings::EVerticalAlignment m_defaultVerticalAlignment = Settings::EVerticalAlignment::Top;
        bool m_ignoreConfigFile = true;

        Math::FVector2 m_position = Math::FVector2(0.0f, 0.0f);
        Math::FVector2 m_size = Math::FVector2(0.0f, 0.0f);

        bool m_positionChanged = false;
        bool m_sizeChanged = false;

        Settings::EHorizontalAlignment m_horizontalAlignment = Settings::EHorizontalAlignment::Left;
        Settings::EVerticalAlignment m_verticalAlignment     = Settings::EVerticalAlignment::Top;

        bool m_alignmentChanged = false;
        bool m_firstFrame = true;

        bool m_autoSize = true;

    private:
        Math::FVector2 calculatePositionAlignmentOffset(bool default = false);

        void updatePosition();
        void updateSize();
        void copyImGuiPosition();
        void copyImGuiSize();
    };
}

#endif // ENGINE_UI_PANELS_PANEL_TRANSFORMABLE_HPP_
