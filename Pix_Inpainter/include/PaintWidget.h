#pragma once

#include "ui_PaintWidget.h"

#include "IUiToolStrategy.h"
#include "PaintController.h"
#include "Enums.h"

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QStack>
#include <QPoint>
#include <QPen>

#include <memory>
#include <deque>

namespace paint
{
    class PaintWidget : public QWidget
    {
        Q_OBJECT

    public:
        static constexpr int MAX_UNDO = 10;
        static constexpr qreal MIN_ZOOM = 0.2;
        static constexpr qreal MAX_ZOOM = 8.2;
        static constexpr qreal BASE_ZOOM = 2.0;
        static constexpr qreal ZOOM_INCREMENT = 0.2;
        static constexpr int DEFAULT_CANVAS_WIDTH = 256;
        static constexpr int DEFAULT_CANVAS_HEIGHT = 256;
        static constexpr int DEFAULT_GRID_SIZE = 8;
        static constexpr int ERASER_SIZE_MULTIPLIER = 4;

    public:
        PaintWidget(QWidget* parent = nullptr,
            int width = DEFAULT_CANVAS_WIDTH,
            int height = DEFAULT_CANVAS_HEIGHT,
            float initialZoom = BASE_ZOOM);
        ~PaintWidget();

        PaintWidget(const PaintWidget&) = delete;
        PaintWidget& operator=(const PaintWidget&) = delete;

        PaintWidget(PaintWidget&&) = delete;
        PaintWidget& operator=(PaintWidget&&) = delete;

        void changePrimaryColor(const QColor& color);
        void changeSecondaryColor(const QColor& color);
        void changePenSize(int size);
        void changePenColor(const QColor& color);
        void usePenPrimaryColor();
        void usePenSecondaryColor();
        void undo();
        void redo();
        void resetZoom();
        void zoomIn();
        void zoomOut();
        void clearCanvas();
        void setTool(Tool tool);
        void loadImage(const QImage& image);
        void toggleGrid(bool show);
        void setGridSize(int size);

        QPoint toCanvasPos(const QPoint& widgetPos) const;
        const QColor& getPrimaryColor() const;
        const QColor& getSecondaryColor() const;
        const QPen& getCurrentPen() const;
        const QImage& getCanvasImage() const;
        qreal getZoomLevel() const;

        void updateToolCursor();

        void setController(PaintController* controller);

        void updateCanvas();

        void notifyColorPicked(const QColor& color, bool leftButton);

    signals:
        void colorPicked(const QColor& color, bool leftButton);
        void zoomChanged(qreal zoomLevel);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        QSize sizeHint() const override;

    private:
        Ui::PaintWidgetClass m_ui;

        QImage m_canvasImage;
        QPixmap m_displayPixmap;

        QColor m_primaryColor;
        QColor m_secondaryColor;
        QPen m_pen;

        std::deque<QImage> m_undoStack;
        std::deque<QImage> m_redoStack;

        qreal m_zoom;

        IUiToolStrategyUniquePtr m_currentUiToolStrategy;

        bool m_showGrid;
        int m_gridSize;

        PaintController* m_controller;
    };
}