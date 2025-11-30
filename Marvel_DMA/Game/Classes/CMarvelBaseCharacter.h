#pragma once
#include "CPawn.h"
#include "SDK/Marvel_classes.hpp"

inline const std::unordered_map<int32_t, std::string> hero_id_map = {
	{1011, "Hulk"},
	{1014, "The Punisher"},
	{1015, "Storm"},
	{1016, "Loki"},
	{1017, "Human Torch"},
	{1018, "Dr. Strange"},
	{1020, "Mantis"},
	{1021, "Hawkeye"},
	{1022, "Captain America"},
	{1023, "Rocket Racoon"},
	{1024, "Hela"},
	{1025, "Dagger"},
	{1026, "Black Panther"},
	{1027, "Groot"},
	{1028, "Ultron"},
	{1029, "Magik"},
	{1030, "Moon Knight"},
	{1031, "Luna Snow"},
	{1032, "Squirrel Girl"},
	{1033, "Black Widow"},
	{1034, "Ironman"},
	{1035, "Venom"},
	{1036, "Spider-Man"},
	{1037, "Magneto"},
	{1038, "Scarlet Witch"},
	{1039, "Thor"},
	{1040, "Mr. Fantastic"},
	{1041, "Winter Soldier"},
	{1042, "Peni Parker"},
	{1043, "Star Lord"},
	{1044, "Blade"},
	{1045, "Namor"},
	{1046, "Adam Warlock"},
	{1047, "Jeff The Land Shark"},
	{1048, "Psylocke"},
	{1049, "Wolverine"},
	{1050, "Invisible Woman"},
	{1051, "The Thing"},
	{1052, "Iron Fist"},
	{1053, "Emma Frost"},
	{1054, "Phoenix"},
	{1055, "Daredevil"},
	{1056, "Angela"},
	{1057, "Professor X"},
	{1058, "Gambit"},
	{1059, "Elsa Bloodstone"},
	{1060, "Colossus"},
	{1061, "Locus"},
	{1062, "Beast"},
	{1063, "Nightcrawler"},
	{1064, "Jubilee"},
	{1065, "Rogue"},
	{1067, "Armor"},
	{1068, "Hive"},
	{1069, "White Fox"},
	{1070, "Forge"},
	{1071, "Amaranth"},
	{4011, "Spider Zero"},
	{4012, "Master Weaver"},
	{4016, "Galacta Bot"},
	{4017, "Galacta"},
	{1018, "Galacta Bot (Large)"},
	{4019, "Ultron Drone"},
};

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