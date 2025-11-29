#include "pch.h"

#include "Entity List.h"

#include "Game/Marvel.h"

#include "Player List/Player List.h"

std::vector<uintptr_t> m_AllActors{};
std::vector<uint32_t> m_TypeCheck{};
void EntityList::ReadAllActorAddresses(DMA_Connection* Conn)
{
	auto& Actors = Marvel::m_Actors;
	auto& ActorData = Actors.Data;
	auto& ActorNum = Actors.NumElements;

	if (ActorData == nullptr) [[unlikely]] return;
	if (ActorNum == 0) [[unlikely]] return;

	m_AllActors.resize(ActorNum);

	VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Marvel::RivalsProc.GetPID(), VMMDLL_FLAG_NOCACHE);
	size_t ReadSize = ActorNum * sizeof(uintptr_t);
	DWORD BytesRead = 0;
	VMMDLL_Scatter_PrepareEx(vmsh, reinterpret_cast<uintptr_t>(Actors.GetDataPtr()), static_cast<DWORD>(ReadSize), reinterpret_cast<BYTE*>(m_AllActors.data()), &BytesRead);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Marvel::RivalsProc.GetPID(), VMMDLL_FLAG_NOCACHE);

	if (BytesRead != ReadSize) [[unlikely]]
	{
		std::println("[Entity List] Incomplete read! {}/{}", BytesRead, ReadSize);
		m_AllActors.clear();
		return;
	}

	std::println("[Entity List] Updated {} actor addresses.", m_AllActors.size());

	m_TypeCheck.resize(m_AllActors.size());

	for (size_t i = 0; i < m_AllActors.size(); i++)
	{
		uintptr_t TypeFlagsPtr = m_AllActors[i] + Offsets::Actor::TypeCheck;
		VMMDLL_Scatter_PrepareEx(vmsh, TypeFlagsPtr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_TypeCheck[i]), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	PlayerList::m_PlayerAddresses.clear();

	for (size_t i = 0; i < m_AllActors.size(); i++)
	{
		auto& ActorAddr = m_AllActors[i];
		auto& TypeCheck = m_TypeCheck[i];

		if (TypeCheck == 0x100)
			PlayerList::m_PlayerAddresses.push_back(ActorAddr);
	}

	std::println("[Entity List] Found {} player characters.", PlayerList::m_PlayerAddresses.size());
}
