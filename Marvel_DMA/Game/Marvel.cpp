#include "pch.h"
#include "DMA/DMA.h"
#include "Marvel.h"
#include "Game/Offsets/Offsets.h"

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

	std::println("Successfully updated rarely changed addresses:\n  GWorld: {0:X}\n  Persistent Level: {1:X}", m_GWorldAddress, m_PersistentLevelAddress);
}

void Marvel::UpdateActorArray(DMA_Connection* Conn)
{
	if (!m_PersistentLevelAddress) [[unlikely]]
		return;

	uintptr_t ActorArrayPtr = m_PersistentLevelAddress + offsetof(SDK::ULevel, Actors) + offsetof(SDK::TArray<uintptr_t>, Data);
	m_Actors = Marvel::RivalsProc.ReadMem<SDK::TArray<uintptr_t>>(Conn, ActorArrayPtr);

	std::println("Successfully updated actor array. Actor count: {}", m_Actors.Num());
}