#pragma once

#include <memory>

class QMouseEvent;
class QPainter;

namespace paint
{
    class PaintWidget;
    class IUiToolStrategy;
    using IUiToolStrategyUniquePtr = std::unique_ptr<IUiToolStrategy>;

    class IUiToolStrategy
    {
    public:
        virtual ~IUiToolStrategy() = default;

        virtual void onMousePress(PaintWidget* widget, QMouseEvent* event) = 0;
        virtual void onMouseMove(PaintWidget* widget, QMouseEvent* event) = 0;
        virtual void onMouseRelease(PaintWidget* widget, QMouseEvent* event) = 0;
        virtual void drawPreview(PaintWidget* widget, QPainter& painter) = 0;
        virtual void updateCursor(PaintWidget* widget) = 0;
    };
}