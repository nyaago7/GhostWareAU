#pragma once

#include "il2cpp-appdata.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <set>
#include <queue>
#include <optional>

using namespace app;

namespace GWState
{
	extern HWND Window;
	extern bool ImGuiInitialized;
	extern bool ShowMenu;

	extern std::optional<PlayerControl*> MurderTarget;
	extern std::optional<PlayerControl*> VoteTarget;
	extern std::optional<PlayerControl*> KickTarget;
}