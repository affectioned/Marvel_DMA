#pragma once

class ColorPicker
{
public:
	static void Render();

public:
	static inline ImColor FriendlyColor{ 0.0f, 0.8f, 0.0f, 1.0f };
	static inline ImColor EnemyColor{ 0.8f, 0.0f, 0.0f, 1.0f };
};	