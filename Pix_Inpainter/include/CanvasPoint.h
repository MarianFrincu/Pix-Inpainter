#pragma once

#include "ICanvasPoint.h"

#include <QPoint>

namespace paint
{
    class CanvasPoint : public ICanvasPoint
    {
    public:
        CanvasPoint(int x, int y);
        explicit CanvasPoint(const QPoint& point);

        ~CanvasPoint() override = default;
        CanvasPoint(const CanvasPoint&) = default;
        CanvasPoint& operator=(const CanvasPoint&) = default;
        CanvasPoint(CanvasPoint&&) noexcept = default;
        CanvasPoint& operator=(CanvasPoint&&) noexcept = default;


        int x() const override;
        int y() const override;

        QPoint qpoint() const;

        static ICanvasPointPtr create(int x, int y);
        static ICanvasPointPtr create(const QPoint& point);

    private:
        QPoint m_point;
    };
}