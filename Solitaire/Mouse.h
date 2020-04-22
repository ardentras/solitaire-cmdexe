#pragma once

#include <windows.h>

/************************************************************************
* Class: Mouse
*
* Purpose: Implements a container for monitoring cursor selection and
*			position.
*
* Manager functions:
*		All defaults.
*
* Methods:
*		PollInput()
*			Polls the console and detects if a selection is made.
*		RightClick()
*			Simulates a right-click.
*
*		GetCursor()
*			Returns a COORD struct with current cursor coordinates.
*
*************************************************************************/

class Mouse
{
private:
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor;
	CONSOLE_SELECTION_INFO csi;
public:
	bool PollInput();
	void RightClick();

	COORD GetCursor() const { return cursor; }
};

