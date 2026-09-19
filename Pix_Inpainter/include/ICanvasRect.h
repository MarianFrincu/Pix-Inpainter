#pragma once

#include "ICanvasPoint.h"

#include <memory>

namespace paint
{
    class ICanvasRect;
    using ICanvasRectPtr = std::shared_ptr<ICanvasRect>;
    using ICanvasRectConstPtr = std::shared_ptr<const ICanvasRect>;

    class ICanvasRect 
    {
    public:
        virtual ~ICanvasRect() = default;

        virtual ICanvasPointConstPtr topLeft() const = 0;
        virtual ICanvasPointConstPtr bottomRight() const = 0;
    };
}