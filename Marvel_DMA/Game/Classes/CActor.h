#pragma once
#include "CBaseEntity.h"
#include "Game/Offsets/Offsets.h"

class CActor : public CBaseEntity
{
public:
	SDK::FVector m_Location{};
	uintptr_t m_RootComponentAddress{};

public:
	CActor(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {};
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CBaseEntity::PrepareRead_1(vmsh);

		if (IsInvalid())
			return;

		uintptr_t RootComponentPtr = m_EntityAddress + offsetof(SDK::AActor, RootComponent);
		VMMDLL_Scatter_PrepareEx(vmsh, RootComponentPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_RootComponentAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	}
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CBaseEntity::PrepareRead_2(vmsh);

		if (IsInvalid())
			return;

		uintptr_t LocationPtr = m_RootComponentAddress + Offsets::SceneComponent::Location;
		VMMDLL_Scatter_PrepareEx(vmsh, LocationPtr, sizeof(SDK::FVector), reinterpret_cast<BYTE*>(&m_Location), reinterpret_cast<DWORD*>(&m_BytesRead));
	}
	void Finalize()
	{
		if (m_BytesRead != sizeof(SDK::FVector))
			SetInvalid();
	}
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (IsInvalid())
			return;

		uintptr_t LocationPtr = m_RootComponentAddress + Offsets::SceneComponent::Location;
		VMMDLL_Scatter_PrepareEx(vmsh, LocationPtr, sizeof(SDK::FVector), reinterpret_cast<BYTE*>(&m_Location), reinterpret_cast<DWORD*>(&m_BytesRead));
	}
	void QuickFinalize()
	{
		if (m_BytesRead != sizeof(SDK::FVector))
			SetInvalid();
	}
};