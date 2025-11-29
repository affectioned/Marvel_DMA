#pragma once
#include "Game/Classes/CMarvelBaseCharacter.h"
#include "DMA/DMA.h"

class PlayerList
{
public:
	static inline std::mutex m_PlayerListMutex;
	static inline std::vector<CMarvelBaseCharacter> m_Players{};
	static inline std::vector<uintptr_t> m_PlayerAddresses{};

public:
	static void FullUpdate(DMA_Connection* Conn);
	static void QuickUpdate(DMA_Connection* Conn);
};