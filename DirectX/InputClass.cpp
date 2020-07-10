#include "pch.h"
#include "InputClass.h"


void InputClass::KeyDown(unsigned int input)
{
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	m_keys[input] = false;
	return;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

void InputClass::UpdateCursorPos(HWND hwnd)
{
	std::call_once(flag, [&]() {
		POINT startPos;
		GetCursorPos(&startPos);
		ScreenToClient(hwnd, &startPos);
		lastCursorPos = startPos;
		currentCursorPos = startPos;
	});

	lastCursorPos = currentCursorPos;
	GetCursorPos(&currentCursorPos);	
	ScreenToClient(hwnd, &currentCursorPos);
	if (currentCursorPos.x < 0 || currentCursorPos.y < 0)
	{
		rightMBDown = false;
	}
}

POINT InputClass::GetCursorMove()
{
	return POINT{ currentCursorPos.x - lastCursorPos.x, currentCursorPos.y - lastCursorPos.y };
}

POINT InputClass::GetCursorPosition()
{
	return POINT{ currentCursorPos.x, currentCursorPos.y };
}

void InputClass::SetRightMBDown(bool down)
{
	rightMBDown = down;
}

bool InputClass::RightMBDown()
{
	return rightMBDown;
}
