#pragma once

#include <QResizeEvent>
#include <QScrollArea>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QWidget>
#include <QPixmap>

namespace paint
{
    class ZoomableImageWidget : public QWidget
    {
        Q_OBJECT

    public:
        static constexpr qreal MIN_ZOOM = 0.2;
        static constexpr qreal MAX_ZOOM = 8.2;
        static constexpr qreal BASE_ZOOM = 1.0;
        static constexpr qreal ZOOM_INCREMENT = 0.2;

    public:
        ZoomableImageWidget(QWidget* parent = nullptr);
        ~ZoomableImageWidget() override;
        
        ZoomableImageWidget(const ZoomableImageWidget&) = delete;
        ZoomableImageWidget& operator=(const ZoomableImageWidget&) = delete;
        
        ZoomableImageWidget(ZoomableImageWidget&&) = delete;
        ZoomableImageWidget& operator=(ZoomableImageWidget&&) = delete;

        void setPixmap(const QPixmap& pixmap);
        QPixmap pixmap() const;

        QSize sizeHint() const override;

    public slots:
        void zoomIn();
        void zoomOut();
        void resetZoom();

    signals:
        void zoomChanged(qreal zoomLevel);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    private:
        QPointF mapWidgetToOriginalImage(const QPointF& widgetPos) const;
        QPointF mapOriginalImageToWidget(const QPointF& imagePos) const;
        
        void adjustView(const QPointF& newViewCenterOnOriginal);
        void centerOn(const QPointF& originalImagePoint);
        void limitViewCenter();

        QPixmap m_originalPixmap;
        qreal m_zoomFactor;
        QPointF m_viewCenterOnOriginal; 
        
        bool m_isPanning;
        QPoint m_lastPanMousePos;
    };
}