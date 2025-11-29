#pragma once
#include "DMA/DMA.h"

struct Vector2
{
	float x{ 0 };
	float y{ 0 };
};

class Camera
{
private:
	static inline std::mutex m_LocalPlayerMutex;
	static inline SDK::FMinimalViewInfo m_ViewInfo{ 0 };	

private:
	static inline uintptr_t m_RootComponentAddr = 0;
	static inline uintptr_t m_PlayerCameraManagerAddr = 0;

public:
	static SDK::FMinimalViewInfo GetViewInfo();

	static void FullUpdate(DMA_Connection* Conn);
	static void QuickUpdate(DMA_Connection* Conn);

	static bool WorldToScreen(SDK::FVector& WorldLocation, Vector2& ScreenLocation);
};