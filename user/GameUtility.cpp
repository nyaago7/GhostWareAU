#include "il2cpp-appdata.h"
#include <imgui.h>
#include <vector>
#include "gameUtility.hpp"

ImVec4 AmongUsColorToImVec4(Color color)
{
	return ImVec4(color.r, color.g, color.b, color.a);
}

std::vector<PlayerControl*> GetAllPlayers()
{
	std::vector<PlayerControl*> players = std::vector<PlayerControl*>();

	auto playerList = (*PlayerControl__TypeInfo).static_fields->AllPlayerControls;

	for (int i = 0; i < List_1_PlayerControl__get_Count(playerList, NULL); i++)
		players.push_back(List_1_PlayerControl__get_Item(playerList, i, NULL));

	return players;
}

GameData_IHEKEPMDGIJ* GetPlayerData(PlayerControl* player)
{
	return PlayerControl_get_Data(player, NULL);
}

int GetGameState()
{
	return (*AmongUsClient__TypeInfo).static_fields->Instance->fields._.GameState;
}