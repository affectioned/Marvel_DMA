#include "pch.h"

#include "ESP.h"
#include "Game/Camera/Camera.h"
#include "Game/Entity List/Player List/Player List.h"

void ESP::Render()
{
	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	DrawPlayers(DrawList, WindowPos);
}

void ESP::DrawPlayers(ImDrawList* DrawList, ImVec2& WindowPos)
{
	std::scoped_lock lock(PlayerList::m_PlayerListMutex);
	for (auto& Player : PlayerList::m_Players)
	{
		if (Player.IsInvalid())
			continue;

		if (Player.m_TypeFlags != 0x100)
			continue;

		Vector2 ScreenPos{};
		if (!Camera::WorldToScreen(Player.m_Location, ScreenPos)) continue;

		std::string HealthText = std::format("{0:.0f}/{1:.0f}", Player.m_CurrentHealth, Player.m_MaxHealth);
		auto TextSize = ImGui::CalcTextSize(HealthText.c_str());
		DrawList->AddText(ImVec2(ScreenPos.x + WindowPos.x - (TextSize.x / 2), ScreenPos.y + WindowPos.y - 20.0f), IM_COL32(255, 255, 255, 255), HealthText.c_str());
	}
}
