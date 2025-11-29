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
		{
			SetInvalid();
			std::println("[CBaseEntity] Invalid entity address: 0x0");
		}
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
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (IsInvalid()) return;

		uintptr_t ClassPtr = m_EntityAddress + offsetof(SDK::UObject, Class);
		VMMDLL_Scatter_PrepareEx(vmsh, ClassPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ClassAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	}
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (m_BytesRead != sizeof(uintptr_t))
			SetInvalid();

		if (IsInvalid()) return;
	}
};