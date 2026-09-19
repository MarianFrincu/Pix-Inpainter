#include "CanvasColor.h"

namespace paint {

	CanvasColor::CanvasColor(int r, int g, int b, int a)
		: m_color(r, g, b, a)
	{
	}

	CanvasColor::CanvasColor(const QColor& color)
		: m_color(color)
	{
	}

	int CanvasColor::red() const
	{
		return m_color.red();
	}

	int CanvasColor::green() const
	{
		return m_color.green();
	}

	int CanvasColor::blue() const
	{
		return m_color.blue();
	}

	int CanvasColor::alpha() const
	{
		return m_color.alpha();
	}

	QColor CanvasColor::toQColor() const
	{
		return m_color;
	}

	ICanvasColorPtr CanvasColor::create(int r, int g, int b, int a)
	{
		return std::make_shared<CanvasColor>(r, g, b, a);
	}

	ICanvasColorPtr CanvasColor::create(const QColor& color)
	{
		return std::make_shared<CanvasColor>(color);
	}
}