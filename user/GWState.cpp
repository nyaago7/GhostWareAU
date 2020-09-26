#include "GWState.hpp"

using namespace app;

namespace GWState
{
	HWND Window;
	bool ImGuiInitialized = false;
	bool ShowMenu = false;

	std::optional<PlayerControl*> MurderTarget = std::nullopt;
	std::optional<PlayerControl*> VoteTarget = std::nullopt;
	std::optional<PlayerControl*> KickTarget = std::nullopt;
}