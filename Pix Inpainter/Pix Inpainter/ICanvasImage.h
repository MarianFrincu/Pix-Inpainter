#pragma once

#include "ICanvasColor.h"

#include <memory>

namespace paint
{
    class ICanvasImage;
    using ICanvasImagePtr = std::shared_ptr<ICanvasImage>;
    using ICanvasImageConstPtr = std::shared_ptr<const ICanvasImage>;

    class ICanvasImage
    {
    public:
        virtual ~ICanvasImage() = default;

        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual ICanvasColorConstPtr pixelAt(int x, int y) const = 0;
        virtual void setPixel(int x, int y, ICanvasColorConstPtr color) = 0;
        virtual ICanvasImagePtr clone() const = 0;

        static ICanvasImagePtr create(int width, int height);
    };
}