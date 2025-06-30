#include "UiToolStrategies.h"

namespace paint
{
    void UiPenStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            widget->usePenPrimaryColor();
        }
        else
        {
            widget->usePenSecondaryColor();
        }

    }

    void UiPenStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
    }

    void UiPenStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
    }

    void UiPenStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
    }

    void UiPenStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(Qt::CrossCursor));
    }

    void UiEraserStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        widget->changePenColor(Qt::white);
        widget->changePenSize(m_previousPenSize * PaintWidget::ERASER_SIZE_MULTIPLIER);
    }

    void UiEraserStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
    }

    void UiEraserStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
        widget->changePenSize(m_previousPenSize);
    }

    void UiEraserStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
    }

    void UiEraserStrategy::updateCursor(PaintWidget* widget)
    {
        m_previousPenSize = widget->getCurrentPen().width();

        int eraserSize = m_previousPenSize * PaintWidget::ERASER_SIZE_MULTIPLIER;
        int cursorSize = static_cast<int>(eraserSize * widget->getZoomLevel());

        QPixmap pixmap(cursorSize + 2, cursorSize + 2);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(QBrush(QColor(255, 255, 255, 180)));

        painter.drawRect(1, 1, cursorSize, cursorSize);

        widget->setCursor(QCursor(pixmap, cursorSize / 2 + 1, cursorSize / 2 + 1));
    }

    void UiRectangleStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            widget->usePenPrimaryColor();
        }
        else
        {
            widget->usePenSecondaryColor();
        }
        m_startPoint = widget->toCanvasPos(event->pos());
        m_endPoint = m_startPoint;
    }

    void UiRectangleStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
        m_endPoint = widget->toCanvasPos(event->pos());
        widget->updateCanvas();
    }

    void UiRectangleStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
        m_startPoint = m_endPoint = QPoint();
        widget->updateCanvas();
    }

    void UiRectangleStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
        if (!m_startPoint.isNull() && !m_endPoint.isNull())
        {
            painter.drawRect(QRect(m_startPoint, m_endPoint));
        }
    }

    void UiRectangleStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(Qt::CrossCursor));
    }

    void UiEllipseStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            widget->usePenPrimaryColor();
        }
        else
        {
            widget->usePenSecondaryColor();
        }
        m_startPoint = widget->toCanvasPos(event->pos());
        m_endPoint = m_startPoint;
    }

    void UiEllipseStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
        m_endPoint = widget->toCanvasPos(event->pos());
        widget->updateCanvas();
    }

    void UiEllipseStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
        m_startPoint = m_endPoint = QPoint();
        widget->updateCanvas();
    }

    void UiEllipseStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
        if (!m_startPoint.isNull() && !m_endPoint.isNull())
        {
            painter.drawEllipse(QRect(m_startPoint, m_endPoint));
        }
    }

    void UiEllipseStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(Qt::CrossCursor));
    }

    void UiLineStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            widget->usePenPrimaryColor();
        }
        else
        {
            widget->usePenSecondaryColor();
        }
        m_startPoint = widget->toCanvasPos(event->pos());
        m_endPoint = m_startPoint;
    }

    void UiLineStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
        m_endPoint = widget->toCanvasPos(event->pos());
        widget->updateCanvas();
    }

    void UiLineStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
        m_startPoint = m_endPoint = QPoint();
        widget->updateCanvas();
    }

    void UiLineStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
        if (!m_startPoint.isNull() && !m_endPoint.isNull())
        {
            painter.drawLine(m_startPoint, m_endPoint);
        }
    }

    void UiLineStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(Qt::CrossCursor));
    }

    void UiEyedropperStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        QPoint pos = widget->toCanvasPos(event->pos());

        const QImage& canvasImage = widget->getCanvasImage();
        if (pos.x() >= 0 && pos.y() >= 0 && pos.x() < canvasImage.width() && pos.y() < canvasImage.height())
        {
            m_pickedColor = canvasImage.pixelColor(pos);
        }
    }

    void UiEyedropperStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
    }

    void UiEyedropperStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
        widget->notifyColorPicked(m_pickedColor, event->button() == Qt::LeftButton);
    }

    void UiEyedropperStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
    }

    void UiEyedropperStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(QIcon(":/icons/Eyedropper.png").pixmap(QSize(30, 30)), 0, 25));
    }

    void UiFillStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            widget->usePenPrimaryColor();
        }
        else
        {
            widget->usePenSecondaryColor();
        }
    }

    void UiFillStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
    }

    void UiFillStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {
    }

    void UiFillStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
    }

    void UiFillStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(QIcon(":/icons/Fill.png").pixmap(QSize(40, 40)), 35, 30));
    }

    void UiTriangleStrategy::onMousePress(PaintWidget* widget, QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            widget->usePenPrimaryColor();
        }
        else
        {
            widget->usePenSecondaryColor();
        }
        m_startPoint = widget->toCanvasPos(event->pos());
        m_endPoint = m_startPoint;
    }

    void UiTriangleStrategy::onMouseMove(PaintWidget* widget, QMouseEvent* event)
    {
        m_endPoint = widget->toCanvasPos(event->pos());
        widget->updateCanvas();
    }

    void UiTriangleStrategy::onMouseRelease(PaintWidget* widget, QMouseEvent* event)
    {

        m_startPoint = m_endPoint = QPoint();
        widget->updateCanvas();
    }

    void UiTriangleStrategy::drawPreview(PaintWidget* widget, QPainter& painter)
    {
        if (!m_startPoint.isNull() && !m_endPoint.isNull())
        {
            m_topPoint = QPoint((m_startPoint.x() + m_endPoint.x()) / 2, m_startPoint.y());
            m_leftPoint = QPoint(m_startPoint.x(), m_endPoint.y());
            m_rightPoint = QPoint(m_endPoint.x(), m_endPoint.y());


            painter.drawLine(m_topPoint, m_leftPoint);
            painter.drawLine(m_leftPoint, m_rightPoint);
            painter.drawLine(m_rightPoint, m_topPoint);
        }
    }

    void UiTriangleStrategy::updateCursor(PaintWidget* widget)
    {
        widget->setCursor(QCursor(Qt::CrossCursor));
    }
}