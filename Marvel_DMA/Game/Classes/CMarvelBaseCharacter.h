#pragma once
#include "CPawn.h"
#include "SDK/Marvel_classes.hpp"
#include "hero_map.hpp"

class CMarvelBaseCharacter : public CPawn
{
public:
	uintptr_t m_ReactivePropertyComponentAddress{ 0 };
	uintptr_t m_CachedAttributeSetAddress{ 0 };
	float m_CurrentHealth{ 0.0f };
	float m_MaxHealth{ 0.0f };
	int32_t m_HeroID{ 0 };
	int32_t m_TeamID{ 0 };

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CPawn::PrepareRead_1(vmsh);

		if (IsInvalid())
			return;

		uintptr_t ReactiveComponentPtr = m_EntityAddress + offsetof(SDK::AMarvelBaseCharacter, ReactivePropertyComponent);
		VMMDLL_Scatter_PrepareEx(vmsh, ReactiveComponentPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ReactivePropertyComponentAddress), nullptr);

		uintptr_t HeroIDPtr = m_EntityAddress + offsetof(SDK::AMarvelBaseCharacter, HeroID);
		VMMDLL_Scatter_PrepareEx(vmsh, HeroIDPtr, sizeof(int32_t), reinterpret_cast<BYTE*>(&m_HeroID), nullptr);
	}
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CPawn::PrepareRead_2(vmsh);

		if (!m_ReactivePropertyComponentAddress)
			SetInvalid();

		if (!m_PlayerStateAddress)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t TeamIDPtr = m_PlayerStateAddress + offsetof(SDK::AMarvelPlayerState, OriginalTeamID);
		VMMDLL_Scatter_PrepareEx(vmsh, TeamIDPtr, sizeof(int32_t), reinterpret_cast<BYTE*>(&m_TeamID), nullptr);

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
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CPawn::QuickRead(vmsh);

		if (m_CachedAttributeSetAddress == 0)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t CurrentHealthPtr = m_CachedAttributeSetAddress + offsetof(SDK::UMarvelBaseAttributeSet, Health) + offsetof(SDK::FGameplayAttributeData, CurrentValue);
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthPtr, sizeof(float), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);

		uintptr_t MaxHealthPtr = m_CachedAttributeSetAddress + offsetof(SDK::UMarvelBaseAttributeSet, MaxHealth) + offsetof(SDK::FGameplayAttributeData, CurrentValue);
		VMMDLL_Scatter_PrepareEx(vmsh, MaxHealthPtr, sizeof(float), reinterpret_cast<BYTE*>(&m_MaxHealth), nullptr);
	}

public:
	const std::string& GetHeroName() const
	{
		static const std::string UnknownHero = "Unknown";
		auto it = hero_id_map.find(m_HeroID);
		if (it != hero_id_map.end())
			return it->second;
		return UnknownHero;
	}
	const bool IsAlive() const
	{
		return m_CurrentHealth > 0.0f;
	}
	const bool IsDead() const
	{
		return !IsAlive();
	}
	const bool IsLocallyControlled() const
	{
		return m_ControllerAddress;
	}
	const bool IsFriendly() const;
};