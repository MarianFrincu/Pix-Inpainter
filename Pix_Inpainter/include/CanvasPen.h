#pragma once

#include "ICanvasPen.h"
#include "CanvasColor.h"

#include <QPen>
#include <memory>

namespace paint
{
    class CanvasPen : public ICanvasPen
    {
    public:
        CanvasPen(ICanvasColorConstPtr color, int width);
        explicit CanvasPen(const QPen& pen);

        ~CanvasPen() override = default;
        CanvasPen(const CanvasPen&) = default;
        CanvasPen& operator=(const CanvasPen&) = default;
        CanvasPen(CanvasPen&&) noexcept = default;
        CanvasPen& operator=(CanvasPen&&) noexcept = default;


        ICanvasColorConstPtr color() const override;
        int width() const override;

        QPen qpen() const;

        static ICanvasPenPtr create(ICanvasColorConstPtr color, int width);
        static ICanvasPenPtr create(const QPen& pen);

    private:
        QPen m_pen;
        ICanvasColorConstPtr m_color;
    };
}