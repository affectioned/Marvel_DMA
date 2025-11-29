#include "pch.h"

#include "GUI/Main Window/Main Window.h"

int main()
{
	std::println("Hello, Marvel_DMA!");

	MainWindow::Initialize();

	while (MainWindow::OnFrame()) {}

	MainWindow::Cleanup();

	return 0;
}