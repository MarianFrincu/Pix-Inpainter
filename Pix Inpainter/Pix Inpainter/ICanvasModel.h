#pragma once

#include "ICanvasPoint.h"
#include "ICanvasPen.h"
#include "ICanvasRect.h"
#include "ICanvasImage.h"
#include "ICanvasColor.h"

#include <memory>
#include <vector>

namespace paint
{
    class ICanvasModel;
    using ICanvasModelPtr = std::shared_ptr<ICanvasModel>;
    using ICanvasModelConstPtr = std::shared_ptr<const ICanvasModel>;

    class ICanvasModel
    {
    public:
        virtual ~ICanvasModel() = default;

        virtual void undo() = 0;
        virtual void redo() = 0;
        virtual bool canUndo() const = 0;
        virtual bool canRedo() const = 0;
        virtual void clear() = 0;
        virtual void saveState() = 0;

        virtual void drawPoint(ICanvasPointConstPtr point, ICanvasPenConstPtr pen) = 0;
        virtual void drawLine(ICanvasPointConstPtr from, ICanvasPointConstPtr to, ICanvasPenConstPtr pen) = 0;
        virtual void drawLines(const std::vector<std::pair<ICanvasPointConstPtr, ICanvasPointConstPtr>>& lines, ICanvasPenConstPtr pen) = 0;
        virtual void drawRect(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) = 0;
        virtual void drawEllipse(ICanvasRectConstPtr rect, ICanvasPenConstPtr pen) = 0;
        virtual void fillPoint(ICanvasPointConstPtr point, ICanvasColorConstPtr fillColor) = 0;

        virtual ICanvasImageConstPtr image() const = 0;
        virtual void loadImage(ICanvasImagePtr image) = 0;
        virtual int width() const = 0;
        virtual int height() const = 0;

        static ICanvasModelPtr create(int width, int height);
    };
}