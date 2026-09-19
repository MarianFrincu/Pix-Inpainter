#pragma once

#include "ICanvasPainter.h"
#include "CanvasImage.h"
#include "CanvasPoint.h"
#include "CanvasPen.h"
#include "CanvasRect.h" 
#include "CanvasColor.h"

namespace paint
{
    class CanvasPainter : public ICanvasPainter
    {
    public:
        explicit CanvasPainter(ICanvasImagePtr image);

        CanvasPainter(const CanvasPainter&) = default;
        CanvasPainter& operator=(const CanvasPainter&) = default;

        CanvasPainter(CanvasPainter&&) noexcept = default;
        CanvasPainter& operator=(CanvasPainter&&) noexcept = default;


        void drawPoint(ICanvasPointConstPtr point, ICanvasPenConstPtr pen) override;
        void drawLine(ICanvasPointConstPtr from, ICanvasPointConstPtr to, ICanvasPenConstPtr pen) override;
        void drawLines(const std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>>& lines, ICanvasPenConstPtr pen) override;
        void drawRect(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) override;
        void drawEllipse(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) override;
        void fillPoint(ICanvasPointConstPtr point, ICanvasColorConstPtr fillColor) override;

    private:
        ICanvasImagePtr m_image;

        CanvasImage* getConcreteImage() const;
    };
}