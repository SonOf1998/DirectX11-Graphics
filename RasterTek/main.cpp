#include "pch.h"

#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass system(800, 600);
	system.Run();
	system.Shutdown();

	return 0;
}