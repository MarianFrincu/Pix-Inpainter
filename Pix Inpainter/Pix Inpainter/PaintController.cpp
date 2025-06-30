#include "PaintController.h"
#include "ToolStrategyFactory.h"

#include "CanvasPoint.h"
#include "CanvasRect.h"
#include "CanvasColor.h"
#include "CanvasPen.h"
#include "CanvasImage.h"

#include <QPainter>

namespace paint
{
    PaintController::PaintController(QObject* parent, ICanvasModelPtr model)
        : QObject(parent)
        , m_model(model)
        , m_currentToolStrategy(nullptr)
    {
        notifyCanvasChanged();
    }

    PaintController::~PaintController()
    {
    }

    void PaintController::drawLine(const QPoint& from, const QPoint& to, const QPen& pen)
    {
        if (!m_model) return;
        m_model->drawLine(toCanvasPoint(from), toCanvasPoint(to), toCanvasPen(pen));
    }

    void PaintController::drawLines(const QVector<QPair<QPoint, QPoint>>& lines, const QPen& pen)
    {
        if (!m_model) return;
        
        std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>> canvasLines;
        canvasLines.reserve(lines.size());
        
        for (const auto& line : lines) 
        {
            canvasLines.push_back({toCanvasPoint(line.first), toCanvasPoint(line.second)});
        }
        
        m_model->drawLines(canvasLines, toCanvasPen(pen));
    }

    void PaintController::drawRect(const QRect& rect, const QPen& pen)
    {
        if (!m_model) return;
        m_model->drawRect(toCanvasRect(rect), toCanvasPen(pen));
    }

    void PaintController::drawEllipse(const QRect& rect, const QPen& pen)
    {
        if (!m_model) return;
        m_model->drawEllipse(toCanvasRect(rect), toCanvasPen(pen));
    }

    void PaintController::drawPoint(const QPoint& point, const QPen& pen)
    {
        if (!m_model) return;
        m_model->drawPoint(toCanvasPoint(point), toCanvasPen(pen));
    }

    void PaintController::fillPoint(const QPoint& point, const QColor& fillColor)
    {
        if (!m_model) return;
        m_model->fillPoint(toCanvasPoint(point), toCanvasColor(fillColor));
    }

    void PaintController::handleMousePress(const QPoint& point, const QPen& pen)
    {
        m_currentToolStrategy->onMousePress(this, point, pen);
    }

    void PaintController::handleMouseMove(const QPoint& point, const QPen& pen)
    {
        m_currentToolStrategy->onMouseMove(this, point, pen);
    }

    void PaintController::handleMouseRelease(const QPoint& point, const QPen& pen)
    {
        m_currentToolStrategy->onMouseRelease(this, point, pen);
    }

    void PaintController::setTool(Tool tool)
    {
        m_currentToolStrategy = ToolStrategyFactory::createStrategy(tool);
    }

    void PaintController::undo()
    {
        if (!m_model) return;
        m_model->undo();
        notifyCanvasChanged();
    }

    void PaintController::redo()
    {
        if (!m_model) return;
        m_model->redo();
        notifyCanvasChanged();
    }

    void PaintController::clear()
    {
        if (!m_model) return;
        m_model->clear();
        notifyCanvasChanged();
    }

    void PaintController::saveState()
    {
        if (!m_model) return;
        m_model->saveState();
    }

    void PaintController::loadImage(const QImage& image)
    {
        if (!m_model) return;

        int width = m_model->image()->width();
        int height = m_model->image()->height();

        QImage img = image.scaled(width, height, Qt::KeepAspectRatio);

        if ((image.width() != width) || (image.height() != height))
        {
            QImage background(width, height, QImage::Format_RGB32);
            background.fill(Qt::white);

            QPainter painter(&background);
            int x = (width - img.width()) / 2;
            int y = (height - img.height()) / 2;
            painter.drawImage(x, y, img);

            img = background;
        }

        ICanvasImagePtr canvasImage = CanvasImage::create(img);
        m_model->loadImage(canvasImage);
        notifyCanvasChanged();
    }

    QImage PaintController::getImage() const
    {
        if (!m_model) return QImage(); 
        return fromCanvasImage(m_model->image());
    }

    bool PaintController::canUndo() const
    {
        return m_model ? m_model->canUndo() : false;
    }

    bool PaintController::canRedo() const
    {
        return m_model ? m_model->canRedo() : false;
    }

    ICanvasPointPtr PaintController::toCanvasPoint(const QPoint& point) const
    {
        return CanvasPoint::create(point);
    }

    ICanvasRectPtr PaintController::toCanvasRect(const QRect& rect) const
    {
        return CanvasRect::create(rect);
    }

    ICanvasColorPtr PaintController::toCanvasColor(const QColor& color) const
    {
        return CanvasColor::create(color);
    }

    ICanvasPenPtr PaintController::toCanvasPen(const QPen& pen) const
    {
        ICanvasColorPtr canvasColor = toCanvasColor(pen.color());
        return CanvasPen::create(canvasColor, pen.width());
    }

    QImage PaintController::fromCanvasImage(ICanvasImageConstPtr canvasImage) const
    {
        if (!canvasImage)
            return QImage();
        
        auto concreteImage = std::dynamic_pointer_cast<const CanvasImage>(canvasImage);
        if (concreteImage) 
        {
            return concreteImage->toQImage(); 
        }
        
        QImage image(canvasImage->width(), canvasImage->height(), QImage::Format_ARGB32);
        image.fill(Qt::transparent); 
        for (int y = 0; y < canvasImage->height(); ++y)
        {
            for (int x = 0; x < canvasImage->width(); ++x)
            {
                ICanvasColorConstPtr colorPtr = canvasImage->pixelAt(x, y);
                if (colorPtr) 
                {
                    auto concreteColor = std::dynamic_pointer_cast<const CanvasColor>(colorPtr);
                    if (concreteColor) 
                    {
                         image.setPixelColor(x, y, concreteColor->toQColor());
                    } 
                    else 
                    {
                        image.setPixelColor(x, y, QColor(colorPtr->red(), colorPtr->green(), colorPtr->blue(), colorPtr->alpha()));
                    }
                }
            }
        }
        return image;
    }
    
    void PaintController::notifyCanvasChanged()
    {
        emit canvasChanged();
    }
}
