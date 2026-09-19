#pragma once

#include "ICanvasColor.h"
#include <QColor>

namespace paint
{
    class CanvasColor : public ICanvasColor
    {
    public:
        CanvasColor(int r, int g, int b, int a = 255);
        explicit CanvasColor(const QColor& color);

        ~CanvasColor() override = default;
        CanvasColor(const CanvasColor&) = default;
        CanvasColor& operator=(const CanvasColor&) = default;
        CanvasColor(CanvasColor&&) noexcept = default;
        CanvasColor& operator=(CanvasColor&&) noexcept = default;


        int red() const override;
        int green() const override;
        int blue() const override;
        int alpha() const override;

        QColor toQColor() const;

        static ICanvasColorPtr create(int r, int g, int b, int a = 255);
        static ICanvasColorPtr create(const QColor& color);

    private:
        QColor m_color;
    };
}