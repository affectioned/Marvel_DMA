#pragma once

class ESP
{
public:
	static void Render();

private:
	static void DrawPlayers(ImDrawList* DrawList, ImVec2& WindowPos);
};