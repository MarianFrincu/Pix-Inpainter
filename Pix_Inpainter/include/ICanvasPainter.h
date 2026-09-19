#pragma once

#include "ICanvasImage.h"
#include "ICanvasPoint.h"
#include "ICanvasPen.h"
#include "ICanvasRect.h"
#include "ICanvasColor.h"

#include <memory>
#include <vector>

namespace paint
{
    class ICanvasPainter;
    using ICanvasPainterUniquePtr = std::unique_ptr<ICanvasPainter>;

    class ICanvasPainter
    {
    public:
        virtual ~ICanvasPainter() = default;

        virtual void drawPoint(ICanvasPointConstPtr point, ICanvasPenConstPtr pen) = 0;
        virtual void drawLine(ICanvasPointConstPtr from, ICanvasPointConstPtr to, ICanvasPenConstPtr pen) = 0;
        virtual void drawLines(const std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>>& lines, ICanvasPenConstPtr pen) = 0;
        virtual void drawRect(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) = 0;
        virtual void drawEllipse(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) = 0;
        virtual void fillPoint(ICanvasPointConstPtr point, ICanvasColorConstPtr fillColor) = 0;

        static ICanvasPainterUniquePtr create(ICanvasImagePtr image);
    };
}