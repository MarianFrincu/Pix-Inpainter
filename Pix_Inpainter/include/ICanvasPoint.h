#pragma once

#include <memory>

namespace paint
{
    class ICanvasPoint;
    using ICanvasPointPtr = std::shared_ptr<ICanvasPoint>;
    using ICanvasPointConstPtr = std::shared_ptr<const ICanvasPoint>;

    class ICanvasPoint
    {
    public:
        virtual ~ICanvasPoint() = default;
        virtual int x() const = 0;
        virtual int y() const = 0;
    };
}