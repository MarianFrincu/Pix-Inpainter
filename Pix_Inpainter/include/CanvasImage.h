#pragma once

#include "ICanvasImage.h"
#include <QImage>
#include <memory>

namespace paint
{
    class CanvasImage : public ICanvasImage
    {
    public:
        CanvasImage(int width, int height);
        explicit CanvasImage(const QImage& image);

        ~CanvasImage() override = default;
        CanvasImage(const CanvasImage&) = default;
        CanvasImage& operator=(const CanvasImage&) = default;
        CanvasImage(CanvasImage&&) noexcept = default;
        CanvasImage& operator=(CanvasImage&&) noexcept = default;

        int width() const override;
        int height() const override;
        ICanvasColorConstPtr pixelAt(int x, int y) const override;
        void setPixel(int x, int y, ICanvasColorConstPtr color) override;
        ICanvasImagePtr clone() const override;

        QImage toQImage() const;

        QImage& getQImage_impl();
        const QImage& getQImage_impl() const;

        static ICanvasImagePtr create(int width, int height);
        static ICanvasImagePtr create(const QImage& image);

    private:
        QImage m_image;
    };
}