#pragma once
#include "il2cpp-appdata.h"
#include <imgui.h>
#include <vector>

using namespace app;

ImVec4 AmongUsColorToImVec4(Color color);

std::vector<PlayerControl*> GetAllPlayers();

GameData_IHEKEPMDGIJ* GetPlayerData(PlayerControl* player);

int GetGameState();