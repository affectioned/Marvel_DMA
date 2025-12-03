#include "pch.h"
#include "DMA/DMA.h"
#include "Marvel.h"
#include "Game/Offsets/Offsets.h"

bool Marvel::Initialize(DMA_Connection* Conn)
{
	Marvel::RivalsProc.GetProcessInfo(Conn);

	Offsets::ResolveAll(Conn);

	return false;
}

void Marvel::UpdateRarelyChangedAddresses(DMA_Connection* Conn)
{
	uintptr_t GWorldPtrAddress = Marvel::RivalsProc.GetBaseAddress() + Offsets::GWorld;
	m_GWorldAddress = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, GWorldPtrAddress);
	if (!m_GWorldAddress) [[unlikely]]
	{
		std::println("Failed to read GWorld address.");
		return;
	}

	uintptr_t PersistentLevelPtr = m_GWorldAddress + offsetof(SDK::UWorld, PersistentLevel);
	m_PersistentLevelAddress = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, PersistentLevelPtr);
	if (!m_PersistentLevelAddress) [[unlikely]]
	{
		std::println("Failed to read PersistentLevel address.");
		return;
	}

	uintptr_t OwningGameInstancePtr = m_GWorldAddress + offsetof(SDK::UWorld, OwningGameInstance);
	m_OwningGameInstance = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, OwningGameInstancePtr);
	if (!m_OwningGameInstance) [[unlikely]]
	{
		std::println("Failed to read OwningGameInstance address.");
		return;
	}
}

void Marvel::UpdateActorArray(DMA_Connection* Conn)
{
	if (!m_PersistentLevelAddress) [[unlikely]]
		return;

	uintptr_t ActorArrayPtr = m_PersistentLevelAddress + offsetof(SDK::ULevel, Actors) + offsetof(SDK::TArray<uintptr_t>, Data);
	m_Actors = Marvel::RivalsProc.ReadMem<SDK::TArray<uintptr_t>>(Conn, ActorArrayPtr);
}

void Marvel::UpdateLocalPlayerAddress(DMA_Connection* Conn)
{
	if (!m_OwningGameInstance) [[unlikely]]
		return;

	uintptr_t LocalPlayersArrayPtr = m_OwningGameInstance + offsetof(SDK::UGameInstance, LocalPlayers) + offsetof(SDK::TArray<uintptr_t>, Data);
	uintptr_t LocalPlayerArrayAddress = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, LocalPlayersArrayPtr);
	uintptr_t FirstLocalPlayerAddress = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, LocalPlayerArrayAddress);

	m_LocalPlayerAddress = FirstLocalPlayerAddress;
}

bool Marvel::IsFriendly(int32_t TeamID)
{
	std::scoped_lock Lock(LocalPlayerMutex);
	return m_LocalTeamID == TeamID;
}

void Marvel::SetLocalTeamID(int32_t NewTeamID)
{
	std::scoped_lock Lock(LocalPlayerMutex);
	m_LocalTeamID = NewTeamID;
}
