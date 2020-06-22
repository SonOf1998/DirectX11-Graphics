#include "pch.h"
#include "SystemClass.h"

#include "Graphics.h"
#include "InputClass.h"
#include "CBufferNotFoundException.h"
#include "HResultException.h"
#include "Timer.h"

SystemClass::SystemClass(UINT width, UINT height)
{
	if (FULLSCREEN)
	{
		screenWidth = width;
		screenHeight = height;
		windowWidth = 1280;
		windowHeight = 720;
	}
	else
	{
		GetScreenResolution(screenWidth, screenHeight);
		windowWidth = width;
		windowHeight = height;
	}


	OpenWindow();
	try 
	{
		graphics = std::make_unique<Graphics>(screenWidth, screenHeight, hwnd);	
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



void SystemClass::OpenWindow()
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
			graphics->Resize(screenWidth, screenHeight, fullscreen);
			SetWindowPos(hwnd, NULL, 0, 0, screenWidth, screenHeight, 0);
		}
	}
	if ((wparam == VK_F11 || wparam == 'F') && umsg == WM_KEYDOWN)
	{
		GetScreenResolution(screenWidth, screenHeight);

		try
		{
			fullscreen = !fullscreen;

			if (fullscreen)
			{
				graphics->Resize(screenWidth, screenHeight, fullscreen);
				SetWindowPos(hwnd, NULL, 0, 0, screenWidth, screenHeight, 0);
			}
			else
			{
				int posX = (screenWidth - windowWidth) / 2;
				int posY = (screenHeight - windowHeight) / 2;

				graphics->Resize(windowWidth, windowHeight);
				SetWindowPos(hwnd, NULL, posX, posY, windowWidth, windowHeight, 0);
			}			
		}
		catch (HResultException e)
		{
			MessageBox(hwnd, e.GetErrorMsg(), L"RuntimeError", MB_OK);
		}

		messageHandled = true;
	} 
	else if (umsg == WM_SIZE)
	{
		RECT rect;
		if (GetWindowRect(hwnd, &rect) && graphics != nullptr)
		{
			int newWidth = rect.right - rect.left;
			int newHeight = rect.bottom - rect.top;

			/* Switching to fullscreen trigger WM_SIZE event
			*  which would make us forget the last window size
			*  We skip every WM_SIZE event which would result
			*  in a full screen window.
			*/
			if (newWidth == screenWidth && newHeight == screenHeight)
			{
				messageHandled = true;
				return 0;
			}


			windowWidth = newWidth;
			windowHeight = newHeight;

			try 
			{ 
				graphics->Resize(windowWidth, windowHeight);
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