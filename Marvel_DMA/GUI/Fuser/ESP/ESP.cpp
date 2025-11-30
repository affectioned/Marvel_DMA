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

void ESP::RenderSettings()
{
	ImGui::Checkbox("Player ESP", &bDrawPlayers);
	ImGui::Checkbox("Nametag", &bDrawNametag);
	ImGui::Checkbox("Healthbar", &bDrawHealthbar);
}

void ESP::DrawPlayers(ImDrawList* DrawList, ImVec2& WindowPos)
{
	if (!bDrawPlayers)
		return;

	std::scoped_lock lock(PlayerList::m_PlayerListMutex);
	for (auto& Player : PlayerList::m_Players)
	{
		if (Player.IsInvalid())
			continue;

		Vector2 ScreenPos{};
		if (!Camera::WorldToScreen(Player.m_Location, ScreenPos)) continue;

		int LineNumber = 0;

		if (bDrawNametag)
			DrawPlayerText(Player, DrawList, WindowPos, ScreenPos, LineNumber);

		if (bDrawHealthbar)
			DrawHealthbar(Player, DrawList, WindowPos, ScreenPos, LineNumber);
	}
}

void ESP::DrawPlayerText(CMarvelBaseCharacter& Player, ImDrawList* DrawList, const ImVec2& WindowPos, const Vector2& ScreenPosition, int& LineNumber)
{
	std::string ESPString = std::format("{0:s}", Player.GetHeroName().c_str());
	auto TextSize = ImGui::CalcTextSize(ESPString.c_str());
	DrawList->AddText(ImVec2(ScreenPosition.x + WindowPos.x - (TextSize.x / 2), ScreenPosition.y + WindowPos.y + (LineNumber * TextSize.y)), IM_COL32(255, 255, 255, 255), ESPString.c_str());
	LineNumber++;
}

constexpr float HealthbarWidth = 60.0f;
constexpr float HalfHealthbarWidth = HealthbarWidth / 2.0f;
constexpr float Padding = 2.0f;
constexpr float EffectiveHealthbarWidth = HealthbarWidth - (Padding * 2.0f);
void ESP::DrawHealthbar(CMarvelBaseCharacter& Player, ImDrawList* DrawList, const ImVec2& WindowPos, const Vector2& ScreenPosition, int& LineNumber)
{
	if (Player.IsDead())
		return;

	ImVec2 HealthbarTopLeft = ImVec2(ScreenPosition.x + WindowPos.x - HalfHealthbarWidth, ScreenPosition.y + WindowPos.y + (LineNumber * ImGui::GetTextLineHeight()));
	ImVec2 HealthbarBottomRight = ImVec2(HealthbarTopLeft.x + HealthbarWidth, HealthbarTopLeft.y + ImGui::GetTextLineHeight());
	DrawList->AddRectFilled(HealthbarTopLeft, HealthbarBottomRight, IM_COL32(55, 55, 55, 125));

	float HealthPercent = Player.m_CurrentHealth / Player.m_MaxHealth;
	ImVec2 HealthFillTopLeft = ImVec2(HealthbarTopLeft.x + Padding, HealthbarTopLeft.y + Padding);
	ImVec2 HealthFillBottomRight = ImVec2(HealthFillTopLeft.x + (EffectiveHealthbarWidth * HealthPercent), HealthbarBottomRight.y - Padding);
	DrawList->AddRectFilled(HealthFillTopLeft, HealthFillBottomRight, IM_COL32(0, 200, 0, 125));

	std::string HealthText = std::format("{0:.0f}", Player.m_CurrentHealth);
	auto TextSize = ImGui::CalcTextSize(HealthText.c_str());
	DrawList->AddText(ImVec2(ScreenPosition.x + WindowPos.x - (TextSize.x / 2), ScreenPosition.y + WindowPos.y + (LineNumber * TextSize.y)), IM_COL32(255, 255, 255, 255), HealthText.c_str());

	LineNumber++;
}
