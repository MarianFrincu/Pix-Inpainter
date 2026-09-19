#include "CanvasImage.h"
#include "CanvasColor.h"
#include <QPainter>

namespace paint
{
    ICanvasImagePtr ICanvasImage::create(int width, int height)
    {
        return CanvasImage::create(width, height);
    }

    ICanvasImagePtr CanvasImage::create(int width, int height)
    {
        return std::make_shared<CanvasImage>(width, height);
    }

    ICanvasImagePtr CanvasImage::create(const QImage& image)
    {
        return std::make_shared<CanvasImage>(image);
    }

    CanvasImage::CanvasImage(int width, int height)
        : m_image(width, height, QImage::Format_ARGB32)
    {
        m_image.fill(Qt::white);
    }

    CanvasImage::CanvasImage(const QImage& image)
        : m_image(image)
    {
        if (m_image.format() != QImage::Format_ARGB32)
        {
            m_image = m_image.convertToFormat(QImage::Format_ARGB32);
        }
    }

    int CanvasImage::width() const
    {
        return m_image.width();
    }

    int CanvasImage::height() const
    {
        return m_image.height();
    }

    ICanvasColorConstPtr CanvasImage::pixelAt(int x, int y) const
    {
        if (m_image.rect().contains(x, y))
        {
            return CanvasColor::create(m_image.pixelColor(x, y));
        }
        return nullptr;
    }

    void CanvasImage::setPixel(int x, int y, ICanvasColorConstPtr color)
    {
        if (m_image.rect().contains(x, y) && color)
        {
            auto concreteColor = std::dynamic_pointer_cast<const CanvasColor>(color);
            if (concreteColor)
            {
                m_image.setPixelColor(x, y, concreteColor->toQColor());
            }
            else
            {
                m_image.setPixelColor(x, y, QColor(color->red(), color->green(), color->blue(), color->alpha()));
            }
        }
    }

    ICanvasImagePtr CanvasImage::clone() const
    {
        return std::make_shared<CanvasImage>(m_image);
    }

    QImage CanvasImage::toQImage() const
    {
        return m_image;
    }

    QImage& CanvasImage::getQImage_impl()
    {
        return m_image;
    }

    const QImage& CanvasImage::getQImage_impl() const
    {
        return m_image;
    }

}