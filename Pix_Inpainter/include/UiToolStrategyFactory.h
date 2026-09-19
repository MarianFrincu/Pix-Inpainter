#pragma once

#include "IUiToolStrategy.h"
#include "Enums.h"

#include <memory>

namespace paint
{
    class UiToolStrategyFactory
    {
    public:
        static IUiToolStrategyUniquePtr createStrategy(Tool toolType);
    };
}

