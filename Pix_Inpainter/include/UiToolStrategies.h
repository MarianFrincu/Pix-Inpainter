#pragma once

#include "IUiToolStrategy.h"
#include "PaintWidget.h"

namespace paint
{
    class UiPenStrategy : public IUiToolStrategy
    {
    public:
        UiPenStrategy() = default;
        ~UiPenStrategy() override = default;
        UiPenStrategy(const UiPenStrategy&) = default;
        UiPenStrategy& operator=(const UiPenStrategy&) = default;
        UiPenStrategy(UiPenStrategy&&) = default;
        UiPenStrategy& operator=(UiPenStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;
    };

    class UiEraserStrategy : public IUiToolStrategy
    {
    public:
        UiEraserStrategy() = default;
        ~UiEraserStrategy() override = default;
        UiEraserStrategy(const UiEraserStrategy&) = default;
        UiEraserStrategy& operator=(const UiEraserStrategy&) = default;
        UiEraserStrategy(UiEraserStrategy&&) = default;
        UiEraserStrategy& operator=(UiEraserStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;

    private:
        int m_previousPenSize;
    };

    class UiRectangleStrategy : public IUiToolStrategy {
    public:
        UiRectangleStrategy() = default;
        ~UiRectangleStrategy() override = default;
        UiRectangleStrategy(const UiRectangleStrategy&) = default;
        UiRectangleStrategy& operator=(const UiRectangleStrategy&) = default;
        UiRectangleStrategy(UiRectangleStrategy&&) = default;
        UiRectangleStrategy& operator=(UiRectangleStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
    };

    class UiEllipseStrategy : public IUiToolStrategy {
    public:
        UiEllipseStrategy() = default;
        ~UiEllipseStrategy() override = default;
        UiEllipseStrategy(const UiEllipseStrategy&) = default;
        UiEllipseStrategy& operator=(const UiEllipseStrategy&) = default;
        UiEllipseStrategy(UiEllipseStrategy&&) = default;
        UiEllipseStrategy& operator=(UiEllipseStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
    };

    class UiLineStrategy : public IUiToolStrategy {
    public:
        UiLineStrategy() = default;
        ~UiLineStrategy() override = default;
        UiLineStrategy(const UiLineStrategy&) = default;
        UiLineStrategy& operator=(const UiLineStrategy&) = default;
        UiLineStrategy(UiLineStrategy&&) = default;
        UiLineStrategy& operator=(UiLineStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
    };

    class UiEyedropperStrategy : public IUiToolStrategy {
    public:
        UiEyedropperStrategy() = default;
        ~UiEyedropperStrategy() override = default;
        UiEyedropperStrategy(const UiEyedropperStrategy&) = default;
        UiEyedropperStrategy& operator=(const UiEyedropperStrategy&) = default;
        UiEyedropperStrategy(UiEyedropperStrategy&&) = default;
        UiEyedropperStrategy& operator=(UiEyedropperStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;

    private:
        QColor m_pickedColor;
    };

    class UiFillStrategy : public IUiToolStrategy {
    public:
        UiFillStrategy() = default;
        ~UiFillStrategy() override = default;
        UiFillStrategy(const UiFillStrategy&) = default;
        UiFillStrategy& operator=(const UiFillStrategy&) = default;
        UiFillStrategy(UiFillStrategy&&) = default;
        UiFillStrategy& operator=(UiFillStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;
    };

    class UiTriangleStrategy : public IUiToolStrategy {
    public:
        UiTriangleStrategy() = default;
        ~UiTriangleStrategy() override = default;
        UiTriangleStrategy(const UiTriangleStrategy&) = default;
        UiTriangleStrategy& operator=(const UiTriangleStrategy&) = default;
        UiTriangleStrategy(UiTriangleStrategy&&) = default;
        UiTriangleStrategy& operator=(UiTriangleStrategy&&) = default;

        void onMousePress(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseMove(PaintWidget* widget, QMouseEvent* event) override;
        void onMouseRelease(PaintWidget* widget, QMouseEvent* event) override;
        void drawPreview(PaintWidget* widget, QPainter& painter) override;
        void updateCursor(PaintWidget* widget) override;

    private:
        QPoint m_startPoint;
        QPoint m_endPoint;
        QPoint m_topPoint;
        QPoint m_leftPoint;
        QPoint m_rightPoint;
    };
}