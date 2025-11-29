#include "pch.h"

#include "Entity List.h"

#include "Game/Marvel.h"

#include "Player List/Player List.h"

void EntityList::ReadAllActorAddresses(DMA_Connection* Conn)
{
	auto& Actors = Marvel::m_Actors;
	auto& ActorData = Actors.Data;
	auto& ActorNum = Actors.NumElements;

	if (ActorData == nullptr) [[unlikely]] return;
	if (ActorNum == 0) [[unlikely]] return;

	m_ActorAddresses.resize(ActorNum);

	VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Marvel::RivalsProc.GetPID(), VMMDLL_FLAG_NOCACHE);
	size_t ReadSize = ActorNum * sizeof(uintptr_t);
	DWORD BytesRead = 0;
	VMMDLL_Scatter_PrepareEx(vmsh, reinterpret_cast<uintptr_t>(Actors.GetDataPtr()), static_cast<DWORD>(ReadSize), reinterpret_cast<BYTE*>(m_ActorAddresses.data()), &BytesRead);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	if (BytesRead != ReadSize) [[unlikely]]
	{
		std::println("[Entity List] Incomplete read! {}/{}", BytesRead, ReadSize);
		m_ActorAddresses.clear();
		return;
	}

	std::println("[Entity List] Updated {} actor addresses.", m_ActorAddresses.size());

	PlayerList::m_PlayerAddresses = m_ActorAddresses;
}
