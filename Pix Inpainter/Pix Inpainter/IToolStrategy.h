#pragma once

#include <memory>

class QPoint;
class QPen;

namespace paint
{
    class PaintController;
    class IToolStrategy;
    using IToolStrategyUniquePtr = std::unique_ptr<IToolStrategy>;

    class IToolStrategy 
    {
    public:
        virtual ~IToolStrategy() = default;

        virtual void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) = 0;
        virtual void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) = 0;
        virtual void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) = 0;
    };
}