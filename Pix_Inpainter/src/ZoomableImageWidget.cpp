#include "ZoomableImageWidget.h"
#include <QApplication>
#include <QPainter>

namespace paint
{
    ZoomableImageWidget::ZoomableImageWidget(QWidget* parent)
        : QWidget(parent)
        , m_zoomFactor(BASE_ZOOM)
        , m_viewCenterOnOriginal(0.5, 0.5)
        , m_isPanning(false)
    {
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        setCursor(Qt::OpenHandCursor);
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    ZoomableImageWidget::~ZoomableImageWidget() = default;

    void ZoomableImageWidget::setPixmap(const QPixmap& pixmap)
    {
        m_originalPixmap = pixmap;
        m_zoomFactor = BASE_ZOOM;
        if (!m_originalPixmap.isNull()) 
        {
            m_viewCenterOnOriginal = QPointF(m_originalPixmap.width() / 2.0, m_originalPixmap.height() / 2.0);
        } 
        else
        {
            m_viewCenterOnOriginal = QPointF(0.0, 0.0);
        }
        limitViewCenter();
        update();
    }

    QPixmap ZoomableImageWidget::pixmap() const
    {
        return m_originalPixmap;
    }

    QSize ZoomableImageWidget::sizeHint() const
    {
        if (!m_originalPixmap.isNull()) 
        {
            return m_originalPixmap.size();
        }
        return QSize(200, 200);
    }

    QPointF ZoomableImageWidget::mapWidgetToOriginalImage(const QPointF& widgetPos) const
    {
        if (m_originalPixmap.isNull() || qFuzzyCompare(m_zoomFactor, 0.0)) 
        {
            return QPointF();
        }
        QPointF topLeftOriginal = m_viewCenterOnOriginal - (QPointF(width(), height()) / (2.0 * m_zoomFactor));
        return topLeftOriginal + (widgetPos / m_zoomFactor);
    }

    QPointF ZoomableImageWidget::mapOriginalImageToWidget(const QPointF& imagePos) const
    {
        if (m_originalPixmap.isNull() || qFuzzyCompare(m_zoomFactor, 0.0)) 
        {
            return QPointF();
        }
        QPointF topLeftOriginal = m_viewCenterOnOriginal - (QPointF(width(), height()) / (2.0 * m_zoomFactor));
        return (imagePos - topLeftOriginal) * m_zoomFactor;
    }

    void ZoomableImageWidget::adjustView(const QPointF& newViewCenterOnOriginal)
    {
        m_viewCenterOnOriginal = newViewCenterOnOriginal;
        limitViewCenter();
        update();
    }

    void ZoomableImageWidget::centerOn(const QPointF& originalImagePoint)
    {
        m_viewCenterOnOriginal = originalImagePoint;
        limitViewCenter();
        update();
    }

    void ZoomableImageWidget::limitViewCenter()
    {
        if (m_originalPixmap.isNull() || qFuzzyCompare(m_zoomFactor, 0.0))
        {
            return;
        }

        qreal halfVisibleWidthOriginal = width() / (2.0 * m_zoomFactor);
        qreal halfVisibleHeightOriginal = height() / (2.0 * m_zoomFactor);

        qreal minX = halfVisibleWidthOriginal;
        qreal maxX = m_originalPixmap.width() - halfVisibleWidthOriginal;
        qreal minY = halfVisibleHeightOriginal;
        qreal maxY = m_originalPixmap.height() - halfVisibleHeightOriginal;

        if (m_originalPixmap.width() * m_zoomFactor < width())
        {
            minX = maxX = m_originalPixmap.width() / 2.0;
        }
        if (m_originalPixmap.height() * m_zoomFactor < height())
        {
            minY = maxY = m_originalPixmap.height() / 2.0;
        }

        m_viewCenterOnOriginal.setX(qBound(minX, m_viewCenterOnOriginal.x(), maxX));
        m_viewCenterOnOriginal.setY(qBound(minY, m_viewCenterOnOriginal.y(), maxY));
    }

    void ZoomableImageWidget::zoomIn()
    {
        if (m_originalPixmap.isNull() || m_zoomFactor >= MAX_ZOOM)
            return;

        QPointF anchorOriginal = m_viewCenterOnOriginal;

        m_zoomFactor += ZOOM_INCREMENT;
        m_zoomFactor = qMin(m_zoomFactor, MAX_ZOOM);

        centerOn(anchorOriginal);
        emit zoomChanged(m_zoomFactor);
    }

    void ZoomableImageWidget::zoomOut()
    {
        if (m_originalPixmap.isNull() || m_zoomFactor <= MIN_ZOOM)
            return;

        QPointF anchorOriginal = m_viewCenterOnOriginal;

        m_zoomFactor -= ZOOM_INCREMENT;
        m_zoomFactor = qMax(m_zoomFactor, MIN_ZOOM);

        centerOn(anchorOriginal);
        emit zoomChanged(m_zoomFactor);
    }

    void ZoomableImageWidget::resetZoom()
    {
        if (m_originalPixmap.isNull())
            return;

        m_zoomFactor = BASE_ZOOM;
        if (!m_originalPixmap.isNull())
        {
            centerOn(QPointF(m_originalPixmap.width() / 2.0, m_originalPixmap.height() / 2.0));
        }
        emit zoomChanged(m_zoomFactor);
    }

    void ZoomableImageWidget::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.fillRect(rect(), palette().color(QPalette::Dark));

        if (m_originalPixmap.isNull() || qFuzzyCompare(m_zoomFactor, 0.0))
        {
            return;
        }

        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        qreal visibleWidthOriginal = width() / m_zoomFactor;
        qreal visibleHeightOriginal = height() / m_zoomFactor;

        QRectF sourceRect(
            m_viewCenterOnOriginal.x() - visibleWidthOriginal / 2.0,
            m_viewCenterOnOriginal.y() - visibleHeightOriginal / 2.0,
            visibleWidthOriginal,
            visibleHeightOriginal
        );

        QRectF targetRect = rect();

        painter.drawPixmap(targetRect, m_originalPixmap, sourceRect);
    }

    void ZoomableImageWidget::wheelEvent(QWheelEvent* event)
    {
        if (m_originalPixmap.isNull()) {
            QWidget::wheelEvent(event);
            return;
        }

        if (event->modifiers() & Qt::ControlModifier)
        {
            QPointF mousePosWidget = event->position();
            QPointF anchorOriginal = mapWidgetToOriginalImage(mousePosWidget);

            qreal oldZoom = m_zoomFactor;
            if (event->angleDelta().y() > 0) {

                if (m_zoomFactor < MAX_ZOOM) 
                {
                    m_zoomFactor += ZOOM_INCREMENT;
                    m_zoomFactor = qMin(m_zoomFactor, MAX_ZOOM);
                }
            } 
            else 
            {
                if (m_zoomFactor > MIN_ZOOM) 
                {
                    m_zoomFactor -= ZOOM_INCREMENT;
                    m_zoomFactor = qMax(m_zoomFactor, MIN_ZOOM);
                }
            }

            if (!qFuzzyCompare(oldZoom, m_zoomFactor)) 
            {
                m_viewCenterOnOriginal = anchorOriginal - (mousePosWidget - rect().center()) / m_zoomFactor;
                limitViewCenter();
                emit zoomChanged(m_zoomFactor);
                update();
            }
            event->accept();
        } 
        else 
        {
            QPoint pixelDelta = event->pixelDelta();
            QPointF deltaOriginal;
            if (!pixelDelta.isNull()) 
            {
                deltaOriginal = QPointF(pixelDelta.x(), pixelDelta.y()) / m_zoomFactor;
            } 
            else
            {
                deltaOriginal = QPointF(event->angleDelta().x(), event->angleDelta().y()) / (8.0 * m_zoomFactor);
            }

            if (!deltaOriginal.isNull())
            {
                adjustView(m_viewCenterOnOriginal - deltaOriginal);
                event->accept();
            }
            else
            {
                QWidget::wheelEvent(event);
            }
        }
    }

    void ZoomableImageWidget::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton && !m_originalPixmap.isNull()) 
        {
            m_isPanning = true;
            m_lastPanMousePos = event->pos();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
        }
        else
        {
            QWidget::mousePressEvent(event);
        }
    }

    void ZoomableImageWidget::mouseMoveEvent(QMouseEvent* event)
    {
        if (m_isPanning && (event->buttons() & Qt::LeftButton))
        {
            QPoint deltaWidget = event->pos() - m_lastPanMousePos;
            QPointF deltaOriginal = QPointF(deltaWidget.x(), deltaWidget.y()) / m_zoomFactor;

            adjustView(m_viewCenterOnOriginal - deltaOriginal);

            m_lastPanMousePos = event->pos();
            event->accept();
        } 
        else 
        {
            QWidget::mouseMoveEvent(event);
        }
    }

    void ZoomableImageWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton && m_isPanning) 
        {
            m_isPanning = false;
            setCursor(Qt::OpenHandCursor);
            event->accept();
        } 
        else 
        {
            QWidget::mouseReleaseEvent(event);
        }
    }

    void ZoomableImageWidget::resizeEvent(QResizeEvent* event)
    {
        QWidget::resizeEvent(event);
        limitViewCenter();
        update();
    }
}