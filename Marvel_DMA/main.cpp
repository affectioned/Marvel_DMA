#include "pch.h"

#include "GUI/Main Window/Main Window.h"
#include "DMA/DMA.h"
#include "Game/Marvel.h"

std::atomic<bool> bRunning{ true };
int main()
{
	std::println("Hello, Marvel_DMA!");

	DMA_Connection* Conn = DMA_Connection::GetInstance();
	Marvel::RivalsProc.GetProcessInfo(Conn);

	MainWindow::Initialize();

	while (MainWindow::OnFrame() && bRunning) {}

	MainWindow::Cleanup();

	return 0;
}