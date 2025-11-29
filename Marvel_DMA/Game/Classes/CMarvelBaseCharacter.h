#pragma once
#include "CActor.h"
#include "SDK/Marvel_classes.hpp"

class CMarvelBaseCharacter : public CActor
{
public:
	uintptr_t m_ReactivePropertyComponentAddress{ 0 };
	uintptr_t m_CachedAttributeSetAddress{ 0 };
	float m_CurrentHealth{ 0.0f };
	float m_MaxHealth{ 0.0f };

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CActor::PrepareRead_1(vmsh);

		if (IsInvalid())
			return;

		uintptr_t ReactiveComponentPtr = m_EntityAddress + offsetof(SDK::AMarvelBaseCharacter, ReactivePropertyComponent);
		VMMDLL_Scatter_PrepareEx(vmsh, ReactiveComponentPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ReactivePropertyComponentAddress), nullptr);
	}
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CActor::PrepareRead_2(vmsh);

		if (!m_ReactivePropertyComponentAddress)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t CachedAttributeSetPtr = m_ReactivePropertyComponentAddress + offsetof(SDK::UReactivePropertyComponent, CachedAttributeSet);
		VMMDLL_Scatter_PrepareEx(vmsh, CachedAttributeSetPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_CachedAttributeSetAddress), nullptr);
	}
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (m_CachedAttributeSetAddress == 0)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t CurrentHealthPtr = m_CachedAttributeSetAddress + offsetof(SDK::UMarvelBaseAttributeSet, Health) + offsetof(SDK::FGameplayAttributeData, CurrentValue);
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthPtr, sizeof(float), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);

		uintptr_t MaxHealthPtr = m_CachedAttributeSetAddress + offsetof(SDK::UMarvelBaseAttributeSet, MaxHealth) + offsetof(SDK::FGameplayAttributeData, CurrentValue);
		VMMDLL_Scatter_PrepareEx(vmsh, MaxHealthPtr, sizeof(float), reinterpret_cast<BYTE*>(&m_MaxHealth), nullptr);
	}
	void Finalize()
	{
		CActor::Finalize();
	}
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CActor::QuickRead(vmsh);

		if (m_CachedAttributeSetAddress == 0)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t CurrentHealthPtr = m_CachedAttributeSetAddress + offsetof(SDK::UMarvelBaseAttributeSet, Health) + offsetof(SDK::FGameplayAttributeData, CurrentValue);
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthPtr, sizeof(float), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);

		uintptr_t MaxHealthPtr = m_CachedAttributeSetAddress + offsetof(SDK::UMarvelBaseAttributeSet, MaxHealth) + offsetof(SDK::FGameplayAttributeData, CurrentValue);
		VMMDLL_Scatter_PrepareEx(vmsh, MaxHealthPtr, sizeof(float), reinterpret_cast<BYTE*>(&m_MaxHealth), nullptr);
	}
};