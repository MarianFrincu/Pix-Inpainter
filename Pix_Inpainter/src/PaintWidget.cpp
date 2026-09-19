#include "PaintWidget.h"
#include "PaintController.h"
#include "UiToolStrategies.h"
#include "UiToolStrategyFactory.h"

#include <QApplication>
#include <qbuffer.h>
#include <QPainter>

namespace paint
{
    PaintWidget::PaintWidget(QWidget* parent, int width, int height, float initialZoom)
        : QWidget(parent)
        , m_pen(QPen(Qt::black, 2))
        , m_primaryColor(Qt::black)
        , m_secondaryColor(Qt::white)
        , m_zoom(initialZoom)
        , m_currentUiToolStrategy(nullptr)
        , m_showGrid(false)
        , m_gridSize(DEFAULT_GRID_SIZE)
        , m_controller(nullptr)
    {
        m_ui.setupUi(this);
        resize(width, height);

        m_canvasImage = QImage(size(), QImage::Format_RGB32);
        m_canvasImage.fill(Qt::white);
        m_displayPixmap = QPixmap::fromImage(m_canvasImage);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        resize(sizeHint());
        updateGeometry();
    }

    void PaintWidget::setController(PaintController* controller)
    {
        m_controller = controller;
        connect(m_controller, &paint::PaintController::canvasChanged,
            this, [this]() {
                m_canvasImage = m_controller->getImage();
                updateCanvas();
            });
    }

    void PaintWidget::undo()
    {
        if (m_controller)
            m_controller->undo();
    }

    void PaintWidget::redo()
    {
        if (m_controller)
            m_controller->redo();
    }

    void PaintWidget::clearCanvas()
    {
        if (m_controller)
            m_controller->clear();
    }

    PaintWidget::~PaintWidget()
    {
    }

    void PaintWidget::changePrimaryColor(const QColor& color)
    {
        m_primaryColor = color;
    }

    void PaintWidget::changeSecondaryColor(const QColor& color)
    {
        m_secondaryColor = color;
    }

    void PaintWidget::changePenSize(int size)
    {
        m_pen.setWidth(size);
    }

    void PaintWidget::changePenColor(const QColor& color)
    {
        m_pen.setColor(color);
    }

    void PaintWidget::usePenPrimaryColor()
    {
        changePenColor(m_primaryColor);
    }

    void PaintWidget::usePenSecondaryColor()
    {
        changePenColor(m_secondaryColor);
    }

    void PaintWidget::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        painter.scale(m_zoom, m_zoom);
        painter.drawPixmap(0, 0, m_displayPixmap);

        if (m_showGrid) {
            painter.setPen(QPen(QColor(200, 200, 200, 120), 1, Qt::DashLine));

            for (int x = 0; x < m_canvasImage.width(); x += m_gridSize) {
                painter.drawLine(x, 0, x, m_canvasImage.height());
            }

            for (int y = 0; y < m_canvasImage.height(); y += m_gridSize) {
                painter.drawLine(0, y, m_canvasImage.width(), y);
            }
        }

        if (m_currentUiToolStrategy)
        {
            painter.save();

            painter.setPen(m_pen);
            m_currentUiToolStrategy->drawPreview(this, painter);

            painter.restore();
        }
    }

    void PaintWidget::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
        {
            if (m_currentUiToolStrategy)
            {
                m_currentUiToolStrategy->onMousePress(this, event);
                m_controller->handleMousePress(toCanvasPos(event->pos()), m_pen);
            }
        }
    }

    void PaintWidget::mouseMoveEvent(QMouseEvent* event)
    {
        if ((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::RightButton))
        {
            if (m_currentUiToolStrategy)
            {
                m_currentUiToolStrategy->onMouseMove(this, event);
                m_controller->handleMouseMove(toCanvasPos(event->pos()), m_pen);
            }
        }
    }

    void PaintWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
        {
            if (m_currentUiToolStrategy)
            {
                m_controller->handleMouseRelease(toCanvasPos(event->pos()), m_pen);
                m_currentUiToolStrategy->onMouseRelease(this, event);
            }
        }
    }

    void PaintWidget::wheelEvent(QWheelEvent* event)
    {
        if (event->modifiers() & Qt::ControlModifier) {
            if (event->angleDelta().y() > 0) {
                zoomIn();
            }
            else
            {
                zoomOut();
            }
            event->accept();
            return;
        }

        QWidget::wheelEvent(event);
    }

    void PaintWidget::resetZoom()
    {
        m_zoom = BASE_ZOOM;
        updateToolCursor();
        resize(sizeHint());
        updateGeometry();
        update();
        emit zoomChanged(m_zoom);
    }

    void PaintWidget::zoomIn()
    {
        qreal newZoom = m_zoom + ZOOM_INCREMENT;
        if (newZoom <= MAX_ZOOM)
        {
            m_zoom = newZoom;
            updateToolCursor();
            resize(sizeHint());
            updateGeometry();
            update();
            emit zoomChanged(m_zoom);
        }
    }

    void PaintWidget::zoomOut()
    {
        qreal newZoom = m_zoom - ZOOM_INCREMENT;
        if (newZoom >= MIN_ZOOM)
        {
            m_zoom = newZoom;
            updateToolCursor();
            resize(sizeHint());
            updateGeometry();
            update();
            emit zoomChanged(m_zoom);
        }
    }

    void PaintWidget::setTool(Tool tool)
    {
        m_currentUiToolStrategy = UiToolStrategyFactory::createStrategy(tool);
        if (m_currentUiToolStrategy)
            m_currentUiToolStrategy->updateCursor(this);
    }

    void PaintWidget::toggleGrid(bool show)
    {
        m_showGrid = show;
        update();
    }

    void PaintWidget::setGridSize(int size)
    {
        m_gridSize = size;
        update();
    }

    QSize PaintWidget::sizeHint() const
    {
        return QSize(int(m_canvasImage.width() * m_zoom), int(m_canvasImage.height() * m_zoom));
    }

    void PaintWidget::updateCanvas()
    {
        m_displayPixmap = QPixmap::fromImage(m_canvasImage);
        resize(sizeHint());
        updateGeometry();
        update();
    }

    void PaintWidget::notifyColorPicked(const QColor& color, bool leftButton)
    {
        emit colorPicked(color, leftButton);
    }

    QPoint PaintWidget::toCanvasPos(const QPoint& widgetPos) const
    {
        return widgetPos / m_zoom;
    }

    void PaintWidget::loadImage(const QImage& image)
    {
        if (m_controller)
            m_controller->loadImage(image);
    }

    const QColor& PaintWidget::getPrimaryColor() const
    {
        return m_primaryColor;
    }

    const QColor& PaintWidget::getSecondaryColor() const
    {
        return m_secondaryColor;
    }

    const QPen& PaintWidget::getCurrentPen() const
    {
        return m_pen;
    }

    const QImage& PaintWidget::getCanvasImage() const
    {
        return m_canvasImage;
    }

    qreal PaintWidget::getZoomLevel() const
    {
        return m_zoom;
    }

    void PaintWidget::updateToolCursor()
    {
        if (m_currentUiToolStrategy)
            m_currentUiToolStrategy->updateCursor(this);
    }
}