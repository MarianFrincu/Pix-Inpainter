#include "CanvasPoint.h"

namespace paint {

	CanvasPoint::CanvasPoint(const QPoint& point)
		: m_point(point)
	{
	}

	CanvasPoint::CanvasPoint(int x, int y)
		: m_point(x, y)
	{
	}

	int CanvasPoint::x() const
	{
		return m_point.x();
	}

	int CanvasPoint::y() const
	{
		return m_point.y();
	}

	QPoint CanvasPoint::qpoint() const
	{
		return m_point;
	}

	ICanvasPointPtr CanvasPoint::create(const QPoint& point)
	{
		return std::make_shared<CanvasPoint>(point);
	}

	ICanvasPointPtr CanvasPoint::create(int x, int y)
	{
		return std::make_shared<CanvasPoint>(x, y);
	}
}