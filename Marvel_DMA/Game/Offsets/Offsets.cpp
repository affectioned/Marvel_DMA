#include "pch.h"
#include "Offsets.h"
#include "DMA/MultiScan.h"
#include "Game/Marvel.h"

void ResolveGWorld(MultiScan& ms)
{
	PatternInfo piGWorld{ "\x48\x89\x15????\x8B\xDA","xxx????xx", "Marvel-Win64-Shipping.exe" };
	auto SectionOffset = ms.ScanSectionOffset(piGWorld);

	auto BufferStart = ms.GetBuffer();
	ms.GetSectionBase();

	uint32_t Displacement = *reinterpret_cast<uint32_t*>(BufferStart + SectionOffset + 3);

	Offsets::GWorld = ms.GetSectionBase() + SectionOffset + 7 + Displacement;
	std::println("GWorldOffset: {0:X}", Offsets::GWorld);
}

bool Offsets::ResolveAll(DMA_Connection* Conn)
{
	MultiScan ms;
	ms.Initialize(Conn->GetHandle(), Marvel::RivalsProc.GetPID(), "Marvel-Win64-Shipping.exe");

	ResolveGWorld(ms);

	ms.Close();

	return false;
}
