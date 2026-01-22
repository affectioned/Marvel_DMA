#include "pch.h"

#include "Entity List.h"

#include "Game/Marvel.h"

#include "Player List/Player List.h"

std::vector<uintptr_t> m_AllActors{};

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

	std::vector<uintptr_t> rootComponents(m_AllActors.size());

	for (size_t i = 0; i < m_AllActors.size(); i++)
	{
		uintptr_t RootComponentPtr = m_AllActors[i] + offsetof(SDK::AActor, RootComponent);
		VMMDLL_Scatter_PrepareEx(vmsh, RootComponentPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&rootComponents[i]), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	PlayerList::m_PlayerAddresses.clear();

	for (size_t i = 0; i < m_AllActors.size(); i++)
	{
		auto& ActorAddr = m_AllActors[i];
		auto& RootComponent = rootComponents[i];

		if (!RootComponent)
			continue;

		PlayerList::m_PlayerAddresses.push_back(ActorAddr);
	}
}