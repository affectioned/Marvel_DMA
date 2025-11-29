#pragma once

class Fuser
{
public:
	static void Render();

public:
	static inline bool bFuser{ true };
	static inline uint8_t MonitorIndex{ 1 };
	static inline bool bBoldText{ false };
	static inline float fTextSize{ 14.0f };

private:
	static inline std::vector<RECT> MonitorRects{};
	static void PopulateMonitors();
	static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData);

public:
	static const float GetCurrentWidth();
	static const float GetCurrentHeight();
};