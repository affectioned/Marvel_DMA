#pragma once
#include "pch.h"

class CBaseEntity
{
public:
	uintptr_t m_EntityAddress{ 0 };
	uintptr_t m_ClassAddress{ 0 };
	uint32_t m_BytesRead{ 0 };
	uint8_t m_Flags{ 0 };

	CBaseEntity(uintptr_t EntityAddress) : m_EntityAddress(EntityAddress) {
		if (!m_EntityAddress)
			SetInvalid();
	};

public:
	void SetInvalid()
	{
		m_Flags |= 0x1;
	}
	bool IsInvalid()
	{
		return m_Flags & 0x1;
	}
};