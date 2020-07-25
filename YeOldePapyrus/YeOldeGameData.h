#pragma once

class VMClassRegistry;
struct StaticFunctionTag;

#include "skse64/GameTypes.h"

namespace YeOldeGameData
{
	bool RegisterFuncs(VMClassRegistry* registry);
}