#include "UiToolStrategyFactory.h"
#include "UiToolStrategies.h"

namespace paint
{
    std::unique_ptr<IUiToolStrategy> UiToolStrategyFactory::createStrategy(Tool toolType)
    {
        switch (toolType)
        {
        case Tool::Pen:
            return std::make_unique<UiPenStrategy>();
        case Tool::Eraser:
            return std::make_unique<UiEraserStrategy>();
        case Tool::Rectangle:
            return std::make_unique<UiRectangleStrategy>();
        case Tool::Ellipse:
            return std::make_unique<UiEllipseStrategy>();
        case Tool::Line:
            return std::make_unique<UiLineStrategy>();
        case Tool::Triangle:
            return std::make_unique<UiTriangleStrategy>();
        case Tool::Eyedropper:
            return std::make_unique<UiEyedropperStrategy>();
        case Tool::Fill:
            return std::make_unique<UiFillStrategy>();
        default:
            return nullptr;
        }
    }
}