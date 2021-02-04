#pragma once

#define FMT_HEADER_ONLY
#include "fmt/format.h"

#define PRINT fmt::print

#define ASSERT(x) if(!(x)) __debugbreak();

