#pragma once

#include "ICanvasColor.h" 

#include <memory>

namespace paint
{
    class ICanvasPen;
    using ICanvasPenPtr = std::shared_ptr<ICanvasPen>;
    using ICanvasPenConstPtr = std::shared_ptr<const ICanvasPen>;

    class ICanvasPen
    {
    public:
        virtual ~ICanvasPen() = default;

        virtual ICanvasColorConstPtr color() const = 0;
        virtual int width() const = 0;
    };
}