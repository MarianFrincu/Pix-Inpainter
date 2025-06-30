#pragma once

#include "ICanvasModel.h" 
#include "ICanvasPainter.h"

#include <deque>
#include <memory>

namespace paint
{
    class CanvasModel : public ICanvasModel
    {
    public:
        static constexpr int MAX_UNDO = 10;

    public:
        explicit CanvasModel(int width, int height);
        ~CanvasModel() override;

        CanvasModel(const CanvasModel&) = delete;
        CanvasModel& operator=(const CanvasModel&) = delete;

        CanvasModel(CanvasModel&& other) noexcept;
        CanvasModel& operator=(CanvasModel&& other) noexcept;


        void undo() override;
        void redo() override;
        bool canUndo() const override;
        bool canRedo() const override;
        void clear() override;
        void saveState() override;

        void drawPoint(ICanvasPointConstPtr point, ICanvasPenConstPtr pen) override;
        void drawLine(ICanvasPointConstPtr from, ICanvasPointConstPtr to, ICanvasPenConstPtr pen) override;
        void drawLines(const std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>>& lines, ICanvasPenConstPtr pen) override;
        void drawRect(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) override;
        void drawEllipse(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) override;
        void fillPoint(ICanvasPointConstPtr point, ICanvasColorConstPtr fillColor) override;

        ICanvasImageConstPtr image() const override;
        void loadImage(ICanvasImagePtr image) override;
        int width() const override;
        int height() const override;

    private:
        ICanvasImagePtr m_image;
        std::deque<ICanvasImagePtr> m_undoStack;
        std::deque<ICanvasImagePtr> m_redoStack;
        ICanvasPainterUniquePtr m_painter;
    };
}