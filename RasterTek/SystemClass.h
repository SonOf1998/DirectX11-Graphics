#pragma once

class Graphics;

class SystemClass
{
	static constexpr bool FULLSCREEN = false;
	UINT screenWidth;
	UINT screenHeight;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND hwnd;

	std::unique_ptr<Graphics> graphics;

	void OpenWindow();

public:
	SystemClass(UINT, UINT);
	SystemClass(const SystemClass&) = delete;
	~SystemClass();

	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);	
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;