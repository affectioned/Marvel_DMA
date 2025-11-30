#pragma once
#include "DMA/Process.h"

class Marvel
{
public:
	static inline Process RivalsProc{};

public:
	static void UpdateRarelyChangedAddresses(DMA_Connection* Conn);
	static inline uintptr_t m_GWorldAddress{ 0 };
	static inline uintptr_t m_PersistentLevelAddress{ 0 };
	static inline uintptr_t m_OwningGameInstance{ 0 };

public:
	static void UpdateActorArray(DMA_Connection* Conn);
	static inline SDK::TArray<uintptr_t> m_Actors{};

public:
	static void UpdateLocalPlayerAddress(DMA_Connection* Conn);
	static inline uintptr_t m_LocalPlayerAddress{ 0 };

private:
	static inline std::mutex LocalPlayerMutex;
	static inline int32_t m_LocalTeamID{};
	
public:
	static bool IsFriendly(int32_t TeamID);
	static void SetLocalTeamID(int32_t NewTeamID);
};