#pragma once

#include "DMA/DMA.h"

class EntityList
{
public:
	static inline std::vector<uintptr_t> m_ActorAddresses{};

public:
	static void ReadAllActorAddresses(DMA_Connection* Conn);
};