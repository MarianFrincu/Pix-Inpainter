#include "CanvasRect.h"
#include "CanvasPoint.h"

namespace paint
{
    CanvasRect::CanvasRect(ICanvasPointConstPtr topLeft, ICanvasPointConstPtr bottomRight)
        : m_rect(QPoint(topLeft->x(), topLeft->y()), QPoint(bottomRight->x(), bottomRight->y()))
    {
    }

    CanvasRect::CanvasRect(const QRect& rect)
        : m_rect(rect)
    {
    }

    ICanvasPointConstPtr CanvasRect::topLeft() const
    {
        return CanvasPoint::create(m_rect.topLeft());
    }

    ICanvasPointConstPtr CanvasRect::bottomRight() const
    {
        return  CanvasPoint::create(m_rect.bottomRight());
    }

    QRect CanvasRect::qrect() const
    {
        return m_rect;
    }

    ICanvasRectPtr CanvasRect::create(ICanvasPointConstPtr topLeft, ICanvasPointConstPtr bottomRight)
    {
        return std::make_shared<CanvasRect>(topLeft, bottomRight);
    }

    ICanvasRectPtr CanvasRect::create(const QRect& rect)
    {
        return std::make_shared<CanvasRect>(rect);
    }
}