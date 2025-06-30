#pragma once

#include <memory>

namespace paint
{
    class ICanvasColor;
    using ICanvasColorPtr = std::shared_ptr<ICanvasColor>;
    using ICanvasColorConstPtr = std::shared_ptr<const ICanvasColor>;
    class ICanvasColor
    {
    public:
        virtual ~ICanvasColor() = default;

        virtual int red() const = 0;
        virtual int green() const = 0;
        virtual int blue() const = 0;
        virtual int alpha() const = 0;
    };
}