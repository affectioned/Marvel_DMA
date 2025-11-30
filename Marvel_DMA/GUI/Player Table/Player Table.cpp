#include "pch.h"
#include "Player Table.h"
#include "Game/Entity List/Player List/Player List.h"
#include "Game/Camera/Camera.h"

void PlayerTable::Render()
{
	std::scoped_lock lock(PlayerList::m_PlayerListMutex);

	ImGui::Begin("Player Table");

	if (ImGui::BeginTable("##player_table", 8))
	{
		ImGui::TableSetupColumn("Player Address");
		ImGui::TableSetupColumn("Location");
		ImGui::TableSetupColumn("Copy Address");
		ImGui::TableSetupColumn("Screen Pos");
		ImGui::TableSetupColumn("Health");
		ImGui::TableSetupColumn("Team");
		ImGui::TableSetupColumn("Contoller");
		ImGui::TableSetupColumn("Friendly?");
		ImGui::TableHeadersRow();

		for (auto& Player : PlayerList::m_Players)
		{
			if (Player.IsInvalid())	continue;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("0x%p", reinterpret_cast<void*>(Player.m_EntityAddress));
			ImGui::TableNextColumn();
			ImGui::Text("%.2f %.2f %.2f ", Player.m_Location.X, Player.m_Location.Y, Player.m_Location.Z);
			ImGui::TableNextColumn();
			if (ImGui::Button(std::format("Copy Address##{}", Player.m_EntityAddress).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", Player.m_EntityAddress).c_str());
			ImGui::TableNextColumn();
			Vector2 ScreenPosition{};
			if (Camera::WorldToScreen(Player.m_Location, ScreenPosition))
				ImGui::Text("X: %.2f Y: %.2f", ScreenPosition.x, ScreenPosition.y);
			else
				ImGui::Text("N/A");
			ImGui::TableNextColumn();
			ImGui::Text("%.2f / %.2f", Player.m_CurrentHealth, Player.m_MaxHealth);
			ImGui::TableNextColumn();
			ImGui::Text("%d", Player.m_TeamID);
			ImGui::TableNextColumn();
			ImGui::Text("%X", Player.m_ControllerAddress);
			ImGui::TableNextColumn();
			ImGui::Text("%d", Player.IsFriendly());
		}

		ImGui::EndTable();
	}

	ImGui::End();
}