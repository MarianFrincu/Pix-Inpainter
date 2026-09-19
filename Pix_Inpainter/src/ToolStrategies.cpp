#include "ToolStrategies.h"
#include "PaintWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QCursor>
#include <QApplication>
#include <QStack>
#include <memory>

namespace paint
{
    void PenStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        controller->saveState();

        m_lastPoint = point;
        controller->drawPoint(m_lastPoint, pen);
        controller->notifyCanvasChanged();
        m_isDrawing = true;
    }

    void PenStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        if (m_isDrawing)
        {
            controller->drawLine(m_lastPoint, point, pen);
            controller->notifyCanvasChanged();
            m_lastPoint = point;
        }
    }

    void PenStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_isDrawing = false;
        controller->drawLine(m_lastPoint, point, pen);
        controller->notifyCanvasChanged();
    }

    void EraserStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        controller->saveState();

        m_lastPoint = point;
        controller->drawPoint(m_lastPoint, pen);
        controller->notifyCanvasChanged();
        m_isDrawing = true;
    }

    void EraserStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        if (m_isDrawing)
        {
            controller->drawLine(m_lastPoint, point, pen);
            controller->notifyCanvasChanged();
            m_lastPoint = point;
        }
    }

    void EraserStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_isDrawing = false;
        controller->drawLine(m_lastPoint, point, pen);
        controller->notifyCanvasChanged();
    }

    void RectangleStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_startPoint = point;
        m_endPoint = m_startPoint;
    }

    void RectangleStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_endPoint = point;
    }

    void RectangleStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        controller->saveState();
        controller->drawRect(QRect(m_startPoint, m_endPoint), pen);
        controller->notifyCanvasChanged();
    }

    void EllipseStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_startPoint = point;
        m_endPoint = m_startPoint;
    }

    void EllipseStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_endPoint = point;
    }

    void EllipseStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        controller->saveState();
        controller->drawEllipse(QRect(m_startPoint, m_endPoint), pen);
        controller->notifyCanvasChanged();
    }

    void LineStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_startPoint = point;
        m_endPoint = m_startPoint;
    }

    void LineStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_endPoint = point;
    }

    void LineStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        controller->saveState();
        controller->drawLine(m_startPoint, m_endPoint, pen);
        controller->notifyCanvasChanged();
    }

    void EyedropperStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
    }

    void EyedropperStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
    }

    void EyedropperStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
    }

    void FillStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        controller->saveState();
        controller->fillPoint(point, pen.color());
        controller->notifyCanvasChanged();
    }

    void FillStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
    }

    void FillStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
    }

    void TriangleStrategy::onMousePress(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_startPoint = point;
        m_endPoint = m_startPoint;
    }

    void TriangleStrategy::onMouseMove(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_endPoint = point;
    }

    void TriangleStrategy::onMouseRelease(PaintController* controller, const QPoint& point, const QPen& pen)
    {
        m_topPoint = QPoint((m_startPoint.x() + m_endPoint.x()) / 2, m_startPoint.y());
        m_leftPoint = QPoint(m_startPoint.x(), m_endPoint.y());
        m_rightPoint = QPoint(m_endPoint.x(), m_endPoint.y());

        QVector<QPair<QPoint, QPoint>> triangleLines = {
            {m_topPoint, m_leftPoint},
            {m_leftPoint, m_rightPoint},
            {m_rightPoint, m_topPoint}
        };

        controller->saveState();
        controller->drawLines(triangleLines, pen);
        controller->notifyCanvasChanged();
    }
}