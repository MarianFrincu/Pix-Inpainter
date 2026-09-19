#pragma once

#include "ICanvasRect.h"

#include <QRect>
#include <memory>

namespace paint
{
    class CanvasRect : public ICanvasRect
    {
    public:
        CanvasRect(ICanvasPointConstPtr topLeft, ICanvasPointConstPtr bottomRight);
        explicit CanvasRect(const QRect& rect);

        ~CanvasRect() override = default;
        CanvasRect(const CanvasRect&) = default;
        CanvasRect& operator=(const CanvasRect&) = default;
        CanvasRect(CanvasRect&&) noexcept = default;
        CanvasRect& operator=(CanvasRect&&) noexcept = default;


        ICanvasPointConstPtr topLeft() const override;
        ICanvasPointConstPtr bottomRight() const override;

        QRect qrect() const;

        static ICanvasRectPtr create(ICanvasPointConstPtr topLeft, ICanvasPointConstPtr bottomRight);
        static ICanvasRectPtr create(const QRect& rect);

    private:
        QRect m_rect;
    };
}