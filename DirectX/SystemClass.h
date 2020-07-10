#pragma once

class Graphics;

class SystemClass
{
	bool fullscreen = FULLSCREEN;

	/* screen maximum resolution */
	int screenWidth;
	int screenHeight;

	/* for fallback purposes when switching back from fullscreen mode */
	UINT windowWidth;	
	UINT windowHeight;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND hwnd;

	std::unique_ptr<Graphics> graphics;

	void OpenWindow();

public:
	SystemClass(UINT, UINT);
	SystemClass(const SystemClass&) = delete;
	~SystemClass();


	static void GetScreenResolution(int& width, int& height);
	
	void Shutdown();
	void Run();
	
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);	
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;