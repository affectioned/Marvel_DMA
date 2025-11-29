#pragma once
#include "DMA/Process.h"
#include "SDK/Engine_classes.hpp"

class Marvel
{
public:
	static inline Process RivalsProc{};

public:
	static void UpdateRarelyChangedAddresses(DMA_Connection* Conn);
	static inline uintptr_t m_GWorldAddress{ 0 };
	static inline uintptr_t m_PersistentLevelAddress{ 0 };

public:
	static void UpdateActorArray(DMA_Connection* Conn);
	static inline SDK::TArray<uintptr_t> m_Actors{};
};