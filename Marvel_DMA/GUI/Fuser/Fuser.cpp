#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"

void Fuser::Render()
{
	if (bFuser == false) return;

	if (MonitorRects.empty()) PopulateMonitors();

	if (MonitorIndex >= MonitorRects.size())
		MonitorIndex = 0;

	auto& MonitorRect = MonitorRects[MonitorIndex];

	ImGui::SetNextWindowSize({ static_cast<float>(MonitorRect.right - MonitorRect.left), static_cast<float>(MonitorRect.bottom - MonitorRect.top) });
	ImGui::SetNextWindowPos({ static_cast<float>(MonitorRect.left), static_cast<float>(MonitorRect.top) });

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	ImGui::Begin("Fuser", &bFuser, window_flags);
	
	ESP::Render();

	ImGui::End();

	ImGui::PopStyleColor(2);
}

void Fuser::PopulateMonitors()
{
	MonitorRects.clear();

	EnumDisplayMonitors(0, 0, MonitorEnum, 0);
}

BOOL CALLBACK Fuser::MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	MonitorRects.push_back(*lprcMonitor);

	return TRUE;
}

const float Fuser::GetCurrentWidth()
{
	return MonitorRects[MonitorIndex].right - MonitorRects[MonitorIndex].left;
}

const float Fuser::GetCurrentHeight()
{
	return MonitorRects[MonitorIndex].bottom - MonitorRects[MonitorIndex].top;
}