#include "pch.h"

#include "ESP.h"
#include "Game/Camera/Camera.h"
#include "Game/Entity List/Player List/Player List.h"

void ESP::Render()
{
	std::scoped_lock lock(PlayerList::m_PlayerListMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	for (auto& Player : PlayerList::m_Players)
	{
		if (Player.IsInvalid())
			continue;

		if (Player.m_TypeFlags != 0x100)
			continue;

		Vector2 ScreenPos{};
		if (!Camera::WorldToScreen(Player.m_Location, ScreenPos)) continue;

		DrawList->AddCircleFilled(ImVec2(ScreenPos.x + WindowPos.x, ScreenPos.y + WindowPos.y), 5.0f, IM_COL32(255, 0, 0, 255));
	}
}