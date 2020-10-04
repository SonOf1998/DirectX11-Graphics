#pragma once

class InputClass
{
	inline static bool m_keys[256] = { false };

	inline static POINT lastCursorPos;
	inline static POINT currentCursorPos;
	inline static bool rightMBDown = false;
	inline static bool leftMBDown = false;

	inline static std::once_flag flag;

public:

	InputClass  ()					= default;
	InputClass  (const InputClass&)	= default;
	~InputClass ()					= default;

	static void KeyDown(unsigned int);
	static void KeyUp(unsigned int);

	static bool IsKeyDown(unsigned int);
	static void UpdateCursorPos(HWND);
	static POINT GetCursorMove();
	static POINT GetCursorPosition();
	static void SetRightMBDown(bool);
	static bool RightMBDown();
	static void SetLeftMBDown(bool);
	static bool LeftMBDown();
};

