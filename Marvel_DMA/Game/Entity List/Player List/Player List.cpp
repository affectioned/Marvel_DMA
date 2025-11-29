#include "pch.h"

#include "Player List.h"

#include "Game/Marvel.h"

void PlayerList::FullUpdate(DMA_Connection* Conn)
{
	std::scoped_lock lock(m_PlayerListMutex);

	m_Players.clear();

	for (auto& Addr : m_PlayerAddresses)
		m_Players.emplace_back(CMarvelBaseCharacter(Addr));

	VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Marvel::RivalsProc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
	{
		if (Player.IsInvalid())
			continue;

		Player.PrepareRead_1(vmsh);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Marvel::RivalsProc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
	{
		if (Player.IsInvalid())
			continue;

		Player.PrepareRead_2(vmsh);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : m_Players)
	{
		if (Player.IsInvalid())
			continue;

		Player.Finalize();
	}
}

void PlayerList::QuickUpdate(DMA_Connection* Conn)
{
	std::scoped_lock lock(m_PlayerListMutex);

	VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Marvel::RivalsProc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Player : m_Players)
	{
		if (Player.IsInvalid())
			continue;

		Player.QuickRead(vmsh);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Player : m_Players)
	{
		Player.QuickFinalize();
	}
}