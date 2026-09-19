#include "ToolStrategyFactory.h"
#include "ToolStrategies.h"

namespace paint
{
    IToolStrategyUniquePtr ToolStrategyFactory::createStrategy(Tool toolType)
    {
        switch (toolType)
        {
        case Tool::Pen:
            return std::make_unique<PenStrategy>();
        case Tool::Eraser:
            return std::make_unique<EraserStrategy>();
        case Tool::Rectangle:
            return std::make_unique<RectangleStrategy>();
        case Tool::Ellipse:
            return std::make_unique<EllipseStrategy>();
        case Tool::Line:
            return std::make_unique<LineStrategy>();
        case Tool::Triangle:
            return std::make_unique<TriangleStrategy>();
        case Tool::Eyedropper:
            return std::make_unique<EyedropperStrategy>();
        case Tool::Fill:
            return std::make_unique<FillStrategy>();
        default:
            return nullptr;
        }
    }
}