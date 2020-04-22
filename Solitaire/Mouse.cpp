/*************************************************************
* Author:			Shaun Rasmusen
* Filename:			Mouse.cpp
* Date Created:		5/17/2016
* Modifications:	None.
************************************************************/
#include "Mouse.h"

/************************************************************************
* Purpose: Polls for a mouse selection in the console.
*
* Precondition:
*		None.
*
* Postcondition:
*		Returns true if a selection is detected and saves coordinates to
*		a COORD struct.
*
************************************************************************/

bool Mouse::PollInput()
{
	bool clicked = false;
	cursor.X = 0;
	cursor.Y = 0;
	SetConsoleCursorPosition(hStdout, cursor);

	GetConsoleSelectionInfo(&csi);
	cursor = csi.dwSelectionAnchor;

	if (csi.dwFlags & CONSOLE_MOUSE_SELECTION)
	{
		clicked = true;

		while (csi.dwFlags != 0)
		{
			RightClick();	// This clears the selection from clicking in the console.
			GetConsoleSelectionInfo(&csi);
		}
	}

	return clicked;
}

/************************************************************************
* Purpose: Simulates a right click.
*
* Precondition:
*		None.
*
* Postcondition:
*		Sends input to the computer simulating a physical right-click.
*
************************************************************************/

void Mouse::RightClick()
{
	POINT pt;
	GetCursorPos(&pt);

	INPUT input = { 0 };
	// right down 
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_ABSOLUTE;
	input.mi.dx = pt.x;
	input.mi.dy = pt.y;
	SendInput(1, &input, sizeof(INPUT));

	// right up
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_ABSOLUTE;
	input.mi.dx = pt.x;
	input.mi.dy = pt.y;
	SendInput(1, &input, sizeof(INPUT));
}