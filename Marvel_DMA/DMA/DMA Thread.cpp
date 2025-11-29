#include "pch.h"

#include "DMA Thread.h"

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

void DMAThread(DMA_Connection* Conn, Process* TF2)
{
#ifdef TRACY_ENABLE
	tracy::SetThreadName("DMA Thread");
#endif


	while (bRunning)
	{
		auto CurrentTime = std::chrono::steady_clock::now();


	}
}