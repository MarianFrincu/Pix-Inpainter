#pragma once

#include "IToolStrategy.h"
#include "Enums.h"

#include <memory>

namespace paint
{
    class ToolStrategyFactory
    {
    public:
        static IToolStrategyUniquePtr createStrategy(Tool toolType);
    };
}