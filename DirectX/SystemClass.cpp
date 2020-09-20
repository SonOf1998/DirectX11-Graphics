#include "pch.h"
#include "SystemClass.h"

#include "Graphics.h"
#include "InputClass.h"
#include "CBufferNotFoundException.h"
#include "HResultException.h"
#include "Timer.h"

SystemClass::SystemClass(UINT width, UINT height)
{
	GetScreenResolution(screenWidth, screenHeight);
	windowWidth = width;
	windowHeight = height;
	


	HWND hwnd = OpenWindow();
	try 
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		int clWidth = rect.right - rect.left;
		int clHeight = rect.bottom - rect.top;

		graphics = std::make_unique<Graphics>(clWidth, clHeight, hwnd);
	}
	catch (const HResultException& e)
	{
		MessageBox(hwnd, e.GetErrorMsg(), L"DirectX setup error", MB_ICONWARNING);
	}
	
}

SystemClass::~SystemClass() = default;



void SystemClass::GetScreenResolution(int& width, int& height)
{
	RECT desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	width = desktop.right;
	height = desktop.bottom;
}

void SystemClass::Shutdown()
{
	if (FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(hwnd);
	UnregisterClass(m_applicationName, m_hinstance);
}


void SystemClass::Run()
{
	Timer timer;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;
	while (!done)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			InputClass::UpdateCursorPos(hwnd);
			timer.Update();

			try
			{
				graphics->RenderFrame(timer.GetT(), timer.GetDt());
			}
			catch (const HResultException& e)
			{
				MessageBox(hwnd, e.GetErrorMsg(), L"DirectX runtime error", MB_ICONWARNING);
			}
			catch (const CBufferNotFoundException& e)
			{
				MessageBoxA(hwnd, e.what(), "CBuffer error", MB_ICONWARNING);
			}
		}
	}
}



HWND SystemClass::OpenWindow()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);
	
	int posX, posY;
	if (FULLSCREEN)
	{
		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)windowWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)windowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		posX = (screenWidth - windowWidth) / 2;
		posY = (screenHeight - windowHeight) / 2;
	}

	hwnd = CreateWindowEx(
		NULL,
		m_applicationName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		posX, posY, windowWidth, windowHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL
	);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return hwnd;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	bool messageHandled = false;

	if (umsg == WM_KEYDOWN)
	{
		InputClass::KeyDown((unsigned int)wparam);
		messageHandled = true;
	}
	if (umsg == WM_KEYUP)
	{
		InputClass::KeyUp((unsigned int)wparam);
		messageHandled = true;
	}
	if (umsg == WM_RBUTTONDOWN)
	{
		InputClass::SetRightMBDown(true);
		messageHandled = true;
	}
	if (umsg == WM_RBUTTONUP)
	{
		InputClass::SetRightMBDown(false);
		messageHandled = true;
	}
	// Window getting focused
	if (umsg == WM_SETFOCUS)
	{
		if (fullscreen) // switching TO fs
		{
			graphics->SwitchMode(fullscreen);
		}

		messageHandled = true;
	}

	if (((wparam == VK_F11 || wparam == 'F') && umsg == WM_KEYDOWN) || wparam == SC_MAXIMIZE )
	{
		fullscreen = !fullscreen;
		graphics->SwitchMode(fullscreen);

		if (!fullscreen)
		{
			int centerX = (screenWidth - windowWidth) / 2;
			int centexY = (screenHeight - windowHeight) / 2;

			SetWindowPos(hwnd, NULL, centerX, centexY, windowWidth, windowHeight, 0);
		}

		messageHandled = true;
	}

	if (umsg == WM_SIZE)
	{
		RECT rect;
		if (GetClientRect(hwnd, &rect) && graphics != nullptr)
		{
			int newWidth = rect.right - rect.left;
			int newHeight = rect.bottom - rect.top;

			/* Switching to fullscreen trigger WM_SIZE event
			*  which would make us forget the last window sizef
			*  We skip every WM_SIZE event which would result
			*  in a full screen window.
			*/
			if (newWidth < screenWidth && newHeight < screenHeight && newWidth * newHeight > 0)
			{
				windowWidth = newWidth;
				windowHeight = newHeight;
			}
			
			try 
			{ 
				if (fullscreen)
				{
					graphics->Resize(screenWidth, screenHeight);					
				}
				else
				{
					graphics->Resize(windowWidth, windowHeight);
				}
			}
			catch (const HResultException& e)
			{
				MessageBox(hwnd, e.GetErrorMsg(), L"RuntimeError", MB_OK);
			}
		}
		messageHandled = true;
	}
	

	if (!messageHandled)
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
	{
		return true;
	}

	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}