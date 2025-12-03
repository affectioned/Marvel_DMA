#pragma once
#include "pch.h"
#include "DMA/DMA.h"

namespace Offsets
{
	bool ResolveAll(DMA_Connection* Conn);

	/*
		48 89 15 ? ? ? ? 8B DA
		4C 8B 35 ? ? ? ? 41 38 5F
		48 8B 1D ? ? ? ? 0F 29 70
		48 8B 0D ? ? ? ? 48 8B 75
		48 8B 1D ? ? ? ? 4D 8B E9
	*/
	inline std::ptrdiff_t GWorld{ 0x0 };

	namespace Actor
	{
		inline std::ptrdiff_t TypeCheck{ 0x38 };
	}
	namespace SceneComponent
	{
		inline std::ptrdiff_t Location{ 0x150 };
	}
}