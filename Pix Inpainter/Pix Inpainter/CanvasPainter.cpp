#include "CanvasPainter.h"

#include <QPainter>
#include <QStack> 

namespace paint 
{
    ICanvasPainterUniquePtr ICanvasPainter::create(ICanvasImagePtr image)
    {
        return std::make_unique<CanvasPainter>(std::move(image));
    }

    CanvasPainter::CanvasPainter(ICanvasImagePtr image)
        : m_image(std::move(image))
    {
    }

    CanvasImage* CanvasPainter::getConcreteImage() const 
    {
        if (!m_image)
            return nullptr;
        return dynamic_cast<CanvasImage*>(m_image.get());
    }

    void CanvasPainter::drawPoint(ICanvasPointConstPtr point, ICanvasPenConstPtr pen)
    {
        CanvasImage* concreteImage = getConcreteImage();
        if (!concreteImage || !point || !pen)
            return;

        auto* canvasPoint = dynamic_cast<const CanvasPoint*>(point.get());
        auto* canvasPen = dynamic_cast<const CanvasPen*>(pen.get());

        if (!canvasPoint || !canvasPen) 
            return;
        
        QPainter painter(&concreteImage->getQImage_impl());
        painter.setPen(canvasPen->qpen());
        painter.drawPoint(canvasPoint->qpoint());
    }

    void CanvasPainter::drawLine(ICanvasPointConstPtr from, ICanvasPointConstPtr to, ICanvasPenConstPtr pen) 
    {
        CanvasImage* concreteImage = getConcreteImage();
        if (!concreteImage || !from || !to || !pen) 
            return;

        auto* canvasFrom = dynamic_cast<const CanvasPoint*>(from.get());
        auto* canvasTo = dynamic_cast<const CanvasPoint*>(to.get());
        auto* canvasPen = dynamic_cast<const CanvasPen*>(pen.get());

        if (!canvasFrom || !canvasTo || !canvasPen) 
            return;
        
        QPainter painter(&concreteImage->getQImage_impl());
        painter.setPen(canvasPen->qpen());
        painter.drawLine(canvasFrom->qpoint(), canvasTo->qpoint());
    }

    void CanvasPainter::drawLines(const std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>>& lines, ICanvasPenConstPtr pen) 
    {
        CanvasImage* concreteImage = getConcreteImage();
        auto* canvasPen = pen ? dynamic_cast<const CanvasPen*>(pen.get()) : nullptr;
        if (!concreteImage || !canvasPen || lines.empty())
            return;
        
        QPainter painter(&concreteImage->getQImage_impl());
        painter.setPen(canvasPen->qpen());
        
        for (const auto& linePair : lines) 
        {
            auto* startPoint = dynamic_cast<const CanvasPoint*>(linePair.first.get());
            auto* endPoint = dynamic_cast<const CanvasPoint*>(linePair.second.get());
            if (!startPoint || !endPoint)
                continue;
            painter.drawLine(startPoint->qpoint(), endPoint->qpoint());
        }
    }

    void CanvasPainter::drawRect(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) 
    {
        CanvasImage* concreteImage = getConcreteImage();
        auto* canvasRect = rect ? dynamic_cast<const CanvasRect*>(rect.get()) : nullptr;
        auto* canvasPen = pen ? dynamic_cast<const CanvasPen*>(pen.get()) : nullptr;

        if (!concreteImage || !canvasRect || !canvasPen) 
            return;
        
        QPainter painter(&concreteImage->getQImage_impl());
        QPen qpenInstance = canvasPen->qpen();
        qpenInstance.setJoinStyle(Qt::MiterJoin); 
        painter.setPen(qpenInstance);
        painter.drawRect(canvasRect->qrect());
    }

    void CanvasPainter::drawEllipse(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen)
    {
        CanvasImage* concreteImage = getConcreteImage();
        auto* canvasRect = rect ? dynamic_cast<const CanvasRect*>(rect.get()) : nullptr;
        auto* canvasPen = pen ? dynamic_cast<const CanvasPen*>(pen.get()) : nullptr;

        if (!concreteImage || !canvasRect || !canvasPen) 
            return;
        
        QPainter painter(&concreteImage->getQImage_impl());
        painter.setPen(canvasPen->qpen());
        painter.drawEllipse(canvasRect->qrect());
    }

    void CanvasPainter::fillPoint(ICanvasPointConstPtr point, ICanvasColorConstPtr fillColor)
    {
        CanvasImage* concreteImage = getConcreteImage();

        ICanvasColorConstPtr targetColor = (concreteImage) ? concreteImage->pixelAt(point->x(), point->y()) : nullptr;
        
        auto* canvasPoint = point ? dynamic_cast<const CanvasPoint*>(point.get()) : nullptr;
        
        if (!concreteImage || !canvasPoint || !targetColor || !fillColor) 
            return;
        
        QImage& img = concreteImage->getQImage_impl();

        QColor qtTargetColor;
        auto concreteTargetColor = std::dynamic_pointer_cast<const CanvasColor>(targetColor);
        if (concreteTargetColor)
            qtTargetColor = concreteTargetColor->toQColor();
        else 
            qtTargetColor = QColor(targetColor->red(), targetColor->green(), targetColor->blue(), targetColor->alpha());

        QColor qtFillColor;
        auto concreteFillColor = std::dynamic_pointer_cast<const CanvasColor>(fillColor);
        if (concreteFillColor) 
            qtFillColor = concreteFillColor->toQColor();
        else 
            qtFillColor = QColor(fillColor->red(), fillColor->green(), fillColor->blue(), fillColor->alpha());


        if (qtTargetColor == qtFillColor)
            return;

        QPoint startQPoint = canvasPoint->qpoint();
        if (!img.rect().contains(startQPoint) || img.pixelColor(startQPoint) != qtTargetColor)
        {
            return; 
        }

        QStack<QPoint> stack;
        stack.push(startQPoint);

        int imgWidth = img.width();
        int imgHeight = img.height();

        while (!stack.isEmpty())
        {
            QPoint p = stack.pop();
            
            if (p.x() < 0 || p.x() >= imgWidth || p.y() < 0 || p.y() >= imgHeight)
                continue;
            
            if (img.pixelColor(p) == qtTargetColor)
            {
                img.setPixelColor(p, qtFillColor);

                if (p.x() + 1 < imgWidth) stack.push(QPoint(p.x() + 1, p.y()));
                if (p.x() - 1 >= 0) stack.push(QPoint(p.x() - 1, p.y()));
                if (p.y() + 1 < imgHeight) stack.push(QPoint(p.x(), p.y() + 1));
                if (p.y() - 1 >= 0) stack.push(QPoint(p.x(), p.y() - 1));
            }
        }
    }
}