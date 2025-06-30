#pragma once

#include "IToolStrategy.h"
#include "PaintController.h"
#include <QPoint>
#include <QPen>

namespace paint
{
    class PenStrategy : public IToolStrategy
    {
    public:
        PenStrategy() = default;
        ~PenStrategy() override = default;
        PenStrategy(const PenStrategy&) = default;
        PenStrategy& operator=(const PenStrategy&) = default;
        PenStrategy(PenStrategy&&) = default;
        PenStrategy& operator=(PenStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;

    private:
        QPoint m_lastPoint;
        bool m_isDrawing = false;
    };

    class EraserStrategy : public IToolStrategy
    {
    public:
        EraserStrategy() = default;
        ~EraserStrategy() override = default;
        EraserStrategy(const EraserStrategy&) = default;
        EraserStrategy& operator=(const EraserStrategy&) = default;
        EraserStrategy(EraserStrategy&&) = default;
        EraserStrategy& operator=(EraserStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;

    private:
        QPoint m_lastPoint;
        bool m_isDrawing = false;
    };

    class RectangleStrategy : public IToolStrategy
    {
    public:
        RectangleStrategy() = default;
        ~RectangleStrategy() override = default;
        RectangleStrategy(const RectangleStrategy&) = default;
        RectangleStrategy& operator=(const RectangleStrategy&) = default;
        RectangleStrategy(RectangleStrategy&&) = default;
        RectangleStrategy& operator=(RectangleStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
    };

    class EllipseStrategy : public IToolStrategy
    {
    public:
        EllipseStrategy() = default;
        ~EllipseStrategy() override = default;
        EllipseStrategy(const EllipseStrategy&) = default;
        EllipseStrategy& operator=(const EllipseStrategy&) = default;
        EllipseStrategy(EllipseStrategy&&) = default;
        EllipseStrategy& operator=(EllipseStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
    };

    class LineStrategy : public IToolStrategy
    {
    public:
        LineStrategy() = default;
        ~LineStrategy() override = default;
        LineStrategy(const LineStrategy&) = default;
        LineStrategy& operator=(const LineStrategy&) = default;
        LineStrategy(LineStrategy&&) = default;
        LineStrategy& operator=(LineStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
    };

    class EyedropperStrategy : public IToolStrategy
    {
    public:
        EyedropperStrategy() = default;
        ~EyedropperStrategy() override = default;
        EyedropperStrategy(const EyedropperStrategy&) = default;
        EyedropperStrategy& operator=(const EyedropperStrategy&) = default;
        EyedropperStrategy(EyedropperStrategy&&) = default;
        EyedropperStrategy& operator=(EyedropperStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;
    };

    class FillStrategy : public IToolStrategy
    {
    public:
        FillStrategy() = default;
        ~FillStrategy() override = default;
        FillStrategy(const FillStrategy&) = default;
        FillStrategy& operator=(const FillStrategy&) = default;
        FillStrategy(FillStrategy&&) = default;
        FillStrategy& operator=(FillStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;
    };

    class TriangleStrategy : public IToolStrategy
    {
    public:
        TriangleStrategy() = default;
        ~TriangleStrategy() override = default;
        TriangleStrategy(const TriangleStrategy&) = default;
        TriangleStrategy& operator=(const TriangleStrategy&) = default;
        TriangleStrategy(TriangleStrategy&&) = default;
        TriangleStrategy& operator=(TriangleStrategy&&) = default;

        void onMousePress(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen) override;
        void onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
        QPoint m_topPoint;
        QPoint m_leftPoint;
        QPoint m_rightPoint;
    };

}