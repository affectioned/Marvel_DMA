#pragma once
#include "Game/Classes/CMarvelBaseCharacter.h"
#include "Game/Camera/Camera.h"

class ESP
{
public:
	static void Render();
	static void RenderSettings();

private:
	static void DrawPlayers(ImDrawList* DrawList, ImVec2& WindowPos);
	static void DrawPlayerText(CMarvelBaseCharacter& Player, ImDrawList* DrawList, const ImVec2& WindowPos, const Vector2& ScreenPosition, int& LineNumber);
	static void DrawHealthbar(CMarvelBaseCharacter& Player, ImDrawList* DrawList, const ImVec2& WindowPos, const Vector2& ScreenPosition, int& LineNumber);

private:
	static inline bool bDrawPlayers{ true };
	static inline bool bDrawNametag{ true };
	static inline bool bDrawHealthbar{ true };
	static inline bool bHideFriendly{ false };
};