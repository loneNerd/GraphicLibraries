#pragma once
#ifndef ENGINE_UI_DRAWABLE_INTERFACE_HPP_
#define ENGINE_UI_DRAWABLE_INTERFACE_HPP_

namespace Engine::UI
{
    class IDrawable
    {
    public:
        virtual ~IDrawable() = default;
        virtual void Draw() = 0;
    };
}

#endif // ENGINE_UI_DRAWABLE_INTERFACE_HPP_
