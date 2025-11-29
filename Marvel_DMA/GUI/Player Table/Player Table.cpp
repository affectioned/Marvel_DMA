#include "pch.h"
#include "Player Table.h"
#include "Game/Entity List/Player List/Player List.h"

void PlayerTable::Render()
{
	std::scoped_lock lock(PlayerList::m_PlayerListMutex);

	ImGui::Begin("Player Table");

	if (ImGui::BeginTable("##player_table", 6))
	{
		ImGui::TableSetupColumn("Player Address");
		ImGui::TableSetupColumn("Location X");
		ImGui::TableSetupColumn("Location Y");
		ImGui::TableSetupColumn("Location Z");
		ImGui::TableSetupColumn("Copy Address");
		ImGui::TableSetupColumn("Flags");
		ImGui::TableHeadersRow();

		for (auto& Player : PlayerList::m_Players)
		{
			if (Player.IsInvalid())
				continue;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("0x%p", reinterpret_cast<void*>(Player.m_EntityAddress));
			ImGui::TableNextColumn();
			ImGui::Text("%.2f", Player.m_Location.X);
			ImGui::TableNextColumn();
			ImGui::Text("%.2f", Player.m_Location.Y);
			ImGui::TableNextColumn();
			ImGui::Text("%.2f", Player.m_Location.Z);
			ImGui::TableNextColumn();
			if (ImGui::Button(std::format("Copy Address##{}", Player.m_EntityAddress).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", Player.m_EntityAddress).c_str());
			ImGui::TableNextColumn();
			ImGui::Text("0x%X", Player.m_TypeFlags);
		}

		ImGui::EndTable();
	}

	ImGui::End();
}