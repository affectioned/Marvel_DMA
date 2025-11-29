#include "pch.h"

#include "DMA Thread.h"

#include "Game/Marvel.h"
#include "Game/Entity List/Entity List.h"
#include "Game/Entity List/Player List/Player List.h"

template <typename T, typename F>
class CTimer
{
public:
	CTimer(T _Interval, F _Function) : m_Interval(_Interval), m_Function(_Function) {}

	void Tick(std::chrono::steady_clock::time_point CurrentTime)
	{
		if (std::chrono::duration_cast<T>(CurrentTime - m_LastExecutionTime) >= m_Interval)
		{
			m_Function();
			m_LastExecutionTime = CurrentTime;
		}
	}
private:
	T m_Interval{};
	F m_Function{};
	std::chrono::steady_clock::time_point m_LastExecutionTime{};
};

extern std::atomic<bool> bRunning;

void DMAThread(DMA_Connection* Conn, Process* MarvelRivals)
{
#ifdef TRACY_ENABLE
	tracy::SetThreadName("DMA Thread");
#endif

	CTimer RarelyChangedAddresses(std::chrono::seconds(5), [Conn]() {Marvel::UpdateRarelyChangedAddresses(Conn); });
	CTimer ActorArrayTimer(std::chrono::seconds(5), [Conn]() {Marvel::UpdateActorArray(Conn); });
	CTimer UpdateActorAddresses(std::chrono::seconds(5), [Conn]() { EntityList::ReadAllActorAddresses(Conn);});
	CTimer UpdatePlayers(std::chrono::seconds(1), [Conn]() { PlayerList::FullUpdate(Conn);});

	while (bRunning)
	{
		auto CurrentTime = std::chrono::steady_clock::now();
		RarelyChangedAddresses.Tick(CurrentTime);
		ActorArrayTimer.Tick(CurrentTime);
		UpdateActorAddresses.Tick(CurrentTime);
		UpdatePlayers.Tick(CurrentTime);
	}
}