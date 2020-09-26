#include "GWGUI.hpp"
using namespace app;

namespace GWGUI
{
	void RenderMenu(bool* state)
	{
		ImGui::Begin("GhostWare - Among Us", state);

		ImGui::SetWindowSize(ImVec2(400, 600));

		if (ImGui::CollapsingHeader("Players") && (GetGameState() == 1 || GetGameState() == 2))
		{
			if (GetAllPlayers().size() > 0)
			{
				for (auto player : GetAllPlayers())
				{
					auto data = GetPlayerData(player);
					auto name = GetUTF8StringFromNETString(data->fields.EKHEPECKPKK);
					ImVec4 nameColor;

					if (data->fields.LODLBBJNGKB)
						nameColor = AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->MLKIANEMBLC);
					else
						nameColor = AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->OOJBHAIPPHP);

					if (data->fields.DMFDFKEJHLH)
						nameColor = AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->DMHEJEACLIG);

					if (ImGui::Button((std::string("Kick") + std::string("##") + name).c_str()))
						GWState::KickTarget = player;

					ImGui::SameLine();

					if (GWState::VoteTarget.has_value() && player == GWState::VoteTarget.value()) {
						if (ImGui::Button((std::string("Next Vote") + std::string("##") + name).c_str()))
							GWState::VoteTarget = std::nullopt;
					}
					else {
						if (ImGui::Button((std::string("Vote Off") + std::string("##") + name).c_str()))
							GWState::VoteTarget = player;
					}

					ImGui::SameLine();

					if (ImGui::Button((std::string("Teleport") + std::string("##") + name).c_str())) {
						Transform* localTransform = Component_get_transform((Component*)(*PlayerControl__TypeInfo).static_fields->LocalPlayer, NULL);
						Transform* playerTransform = Component_get_transform((Component*)player, NULL);
						Transform_set_position(localTransform, Transform_get_position(playerTransform, NULL), NULL);
					}

					ImGui::SameLine();

					if (ImGui::Button((std::string("Murder") + std::string("##") + name).c_str()))
						GWState::MurderTarget = player;

					ImGui::SameLine();

					ImGui::TextColored(nameColor, name.c_str());
				}
			}
		}

		ImGui::End();
	}
}