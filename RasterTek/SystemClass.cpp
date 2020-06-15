#include "pch.h"
#include "SystemClass.h"

#include "Graphics.h"
#include "InputClass.h"
#include "CBufferNotFoundException.h"
#include "HResultException.h"
#include "Timer.h"

SystemClass::SystemClass(UINT screenWidth, UINT screenHeight)
{
	OpenWindow();
	try 
	{
		graphics = std::make_unique<Graphics>(screenWidth, screenHeight, hwnd);	
	}
	catch (HResultException e)
	{
		MessageBox(hwnd, e.GetErrorMsg(), L"DirectX setup error", MB_ICONWARNING);
	}
	
}

SystemClass::~SystemClass() = default;



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
			catch (HResultException e)
			{
				MessageBox(hwnd, e.GetErrorMsg(), L"DirectX runtime error", MB_ICONWARNING);
			}
			catch (CBufferNotFoundException e)
			{
				MessageBoxA(hwnd, e.what(), "CBuffer error", MB_ICONWARNING);
			}
		}

	}
}



void SystemClass::OpenWindow()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


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

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULLSCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	hwnd = CreateWindowEx(
		NULL,
		m_applicationName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL
	);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
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
	if (umsg == WM_SIZE)
	{
		RECT rect;
		if (GetWindowRect(hwnd, &rect) && graphics)
		{
			int newWidth = rect.right - rect.left;
			int newHeight = rect.bottom - rect.top;

			try { graphics->Resize(newWidth, newHeight); }
			catch (HResultException e)
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
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