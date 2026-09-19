#include "CanvasPen.h"

namespace paint
{
    ICanvasPenPtr CanvasPen::create(ICanvasColorConstPtr color, int width)
    {
        return std::make_shared<CanvasPen>(color, width);
    }

    ICanvasPenPtr CanvasPen::create(const QPen& pen)
    {
        return std::make_shared<CanvasPen>(pen);
    }

    CanvasPen::CanvasPen(ICanvasColorConstPtr color, int width)
        : m_color(color)
    {
        QColor qColorInstance;
        if (color)
        {
            auto concreteColor = std::dynamic_pointer_cast<const CanvasColor>(color);
            if (concreteColor)
            {
                qColorInstance = concreteColor->toQColor();
            }
            else
            {
                qColorInstance = QColor(color->red(), color->green(), color->blue(), color->alpha());
            }
        }
        else
        {
            qColorInstance = Qt::black;
        }

        m_pen.setColor(qColorInstance);
        m_pen.setWidth(width);
    }

    CanvasPen::CanvasPen(const QPen& pen)
        : m_pen(pen)
    {
        m_color = CanvasColor::create(pen.color());
    }

    ICanvasColorConstPtr CanvasPen::color() const
    {
        return m_color;
    }

    int CanvasPen::width() const
    {
        return m_pen.width();
    }

    QPen CanvasPen::qpen() const
    {
        return m_pen;
    }

}