#include "CanvasModel.h"

namespace paint
{
    ICanvasModelPtr ICanvasModel::create(int width, int height)
    {
        return std::make_shared<CanvasModel>(width, height);
    }

    CanvasModel::CanvasModel(int width, int height)
        : m_image(ICanvasImage::create(width, height))
        , m_painter(ICanvasPainter::create(m_image))
    {
    }

    CanvasModel::~CanvasModel() = default;

    void CanvasModel::undo()
    {
        if (!canUndo()) return;

        m_redoStack.push_back(m_image);
        m_image = m_undoStack.back();
        m_undoStack.pop_back();
        
        m_painter = ICanvasPainter::create(m_image);
    }

    void CanvasModel::redo()
    {
        if (!canRedo()) return;

        m_undoStack.push_back(m_image);
        m_image = m_redoStack.back();
        m_redoStack.pop_back();
        
        m_painter = ICanvasPainter::create(m_image);
    }
    
    bool CanvasModel::canUndo() const
    {
        return !m_undoStack.empty();
    }
    
    bool CanvasModel::canRedo() const
    {
        return !m_redoStack.empty();
    }

    void CanvasModel::clear()
    {
        saveState();
        m_image = ICanvasImage::create(width(), height());
        m_painter = ICanvasPainter::create(m_image);
    }

    void CanvasModel::saveState()
    {
        if (m_image) 
        {
            m_redoStack.clear();
            m_undoStack.push_back(m_image->clone());
            if (m_undoStack.size() > MAX_UNDO)
            {
                m_undoStack.pop_front();
            }
        }
    }

    void CanvasModel::drawPoint(ICanvasPointConstPtr point, ICanvasPenConstPtr pen)
    {
        if (!m_painter) return;
        m_painter->drawPoint(point, pen);
    }

    void CanvasModel::drawLine(ICanvasPointConstPtr from, ICanvasPointConstPtr to, ICanvasPenConstPtr pen)
    {
        if (!m_painter) return;
        m_painter->drawLine(from, to, pen);
    }

    void CanvasModel::drawLines(const std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>>& lines, ICanvasPenConstPtr pen)
    {
        if (!m_painter) return;
        m_painter->drawLines(lines, pen);
    }

    void CanvasModel::drawRect(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen)
    {
        if (!m_painter) return;
        m_painter->drawRect(rect, pen);
    }

    void CanvasModel::drawEllipse(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen)
    {
        if (!m_painter) return;
        m_painter->drawEllipse(rect, pen);
    }

    void CanvasModel::fillPoint(ICanvasPointConstPtr point, ICanvasColorConstPtr fillColor)
    {
        if (!m_painter) return;
        m_painter->fillPoint(point, fillColor);
    }

    ICanvasImageConstPtr CanvasModel::image() const
    {
        return m_image;
    }

    void CanvasModel::loadImage(ICanvasImagePtr image)
    {
        if (!image) return;
        saveState();
        m_image = image;
        m_painter = ICanvasPainter::create(m_image);
    }

    int CanvasModel::width() const
    {
        return m_image ? m_image->width() : 0;
    }

    int CanvasModel::height() const
    {
        return m_image ? m_image->height() : 0;
    }
}
