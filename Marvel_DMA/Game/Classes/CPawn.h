#pragma once
#include "CActor.h"

class CPawn : public CActor
{
public:
	uintptr_t m_PlayerStateAddress{};
	uintptr_t m_ControllerAddress{};

public:
	CPawn(uintptr_t EntityAddress) : CActor(EntityAddress) {};

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CActor::PrepareRead_1(vmsh);

		if (IsInvalid())
			return;

		uintptr_t PlayerStatePtr = m_EntityAddress + offsetof(SDK::APawn, PlayerState);
		VMMDLL_Scatter_PrepareEx(vmsh, PlayerStatePtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerStateAddress), nullptr);

		uintptr_t ControllerPtr = m_EntityAddress + offsetof(SDK::APawn, Controller);
		VMMDLL_Scatter_PrepareEx(vmsh, ControllerPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ControllerAddress), nullptr);
	}
};