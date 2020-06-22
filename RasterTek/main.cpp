#include "pch.h"

#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	int width = 1280;
	int height = 720;

	if (FULLSCREEN)
	{
		SystemClass::GetScreenResolution(width, height);
	}


	SystemClass system(width, height);
	system.Run();
	system.Shutdown();

	return 0;
}