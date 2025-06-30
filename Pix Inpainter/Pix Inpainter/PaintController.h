#pragma once

#include "ICanvasModel.h"
#include "IToolStrategy.h"
#include "Enums.h"

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QImage>
#include <QPair>
#include <QRect>

namespace paint
{
    class PaintController : public QObject
    {
        Q_OBJECT

    public:
        explicit PaintController(QObject* parent, ICanvasModelPtr model);
        ~PaintController();

        PaintController(const PaintController&) = delete;
        PaintController& operator=(const PaintController&) = delete;

        PaintController(PaintController&&) = delete;
        PaintController& operator=(PaintController&&) = delete;


        void drawLine(const QPoint& from, const QPoint& to, const QPen& pen);
        void drawLines(const QVector<QPair<QPoint, QPoint>>& lines, const QPen& pen);
        void drawRect(const QRect& rect, const QPen& pen);
        void drawEllipse(const QRect& rect, const QPen& pen);
        void drawPoint(const QPoint& point, const QPen& pen);
        void fillPoint(const QPoint& point, const QColor& fillColor);

        void handleMousePress(const QPoint& point, const QPen& pen);
        void handleMouseMove(const QPoint& point, const QPen& pen);
        void handleMouseRelease(const QPoint& point, const QPen& pen);

        void setTool(Tool tool);
        void undo();
        void redo();
        void clear();
        void saveState();
        void loadImage(const QImage& image);

        QImage getImage() const;

        bool canUndo() const;
        bool canRedo() const;

        void notifyCanvasChanged();

    signals:
        void canvasChanged();

    private:
        ICanvasPointPtr toCanvasPoint(const QPoint& point) const;
        ICanvasRectPtr toCanvasRect(const QRect& rect) const;
        ICanvasColorPtr toCanvasColor(const QColor& color) const;
        ICanvasPenPtr toCanvasPen(const QPen& pen) const;
        QImage fromCanvasImage(ICanvasImageConstPtr canvasImage) const;

    private:
        ICanvasModelPtr m_model;
        IToolStrategyUniquePtr m_currentToolStrategy;
    };
}