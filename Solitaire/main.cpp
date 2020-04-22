/**************************************************************
* Author:			Shaun Rasmusen
* Filename:			main.cpp
* Date Created:		5/19/2016
* Modifications:	None.
*
* Lab/Assignment: CST 136, Lab 6, "Solitaire"
*
* Overview: A console version of the classic game of 3-card
*			draw Solitaire.
*	
* Input:
*	The mouse is the sole source of input and can be used to
*	manipulate cards and navigate the game.
*
* Output:
*	A board that is drawn similar to the Windows version of
*	Solitaire with a sidebar containing information and the
*	to either replay or exit the game.
*
**************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <crtdbg.h>

#include "Board.h"
#include "Deck.h"
#include "Mouse.h"

// The game loop for Solitaire
bool Run(Board *& board, const Deck & deck);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Deck deck;
	Board * board = new Board(deck);

	while (!Run(board, deck));

	delete board;

	return 0;
}

/************************************************************************
* Purpose: The primary game loop for Solitaire.
*
* Precondition:
*		* board - A Board object pointer passed by reference.
*		const deck - A Deck object containing a standard 52 card deck.
*
* Postcondition:
*		Handles all logic for the game and collisions involving cursor
*		position and buttons displayed in the console.
*
************************************************************************/

bool Run(Board *& board, const Deck & deck)
{
	Mouse mouse;

	int inSelect = 0;
	time_t lastTime = 0;
	bool exit = false;

	while (!board->GetWin() && !exit)
	{
		if (lastTime < time(0))
		{
			lastTime = time(0);
			board->DisplayTime();
		}
		if (mouse.PollInput())
		{
			COORD cursor = mouse.GetCursor();
			board->SetSelect(inSelect++, cursor.X);
			board->SetSelect(inSelect++, cursor.Y);

			if (cursor.X >= 37 && cursor.X < 42 && cursor.Y < 5)
			{
				board->Deal();
				board->MoveCards();
				inSelect = 0;
			}

			if (cursor.X >= 45 && cursor.X < 53)
			{
				if (cursor.Y >= 31 && cursor.Y < 34)
				{
					delete board;
					board = new Board(deck);
				}

				if (cursor.Y >= 35 && cursor.Y < 38) // Exit
				{
					exit = true;
				}
			}

			if (inSelect == 2)
			{
				if (!board->GetFrom())
					inSelect = 0;
			}

			if (inSelect == 4)
			{
				board->MoveCards();
				inSelect = 0;
			}
		}
	}

	if (board->GetWin())
	{
		bool select = false;

		board->DisplayWin();
		while (!select)
		{
			if (mouse.PollInput())
			{
				COORD cursor = mouse.GetCursor();
				
				if (cursor.X >= 37 && cursor.X < 42 && cursor.Y < 5)
				{
					board->Deal();
					inSelect = 0;
					select = true;
				}

				if (cursor.X >= 45 && cursor.X < 53)
				{
					if (cursor.Y >= 31 && cursor.Y < 34)
					{
						delete board;
						board = new Board(deck);
						select = true;
					}

					if (cursor.Y >= 35 && cursor.Y < 38) // Exit
					{
						exit = true;
						select = true;
					}
				}

				if (inSelect == 2)
				{
					if (!board->GetFrom())
						inSelect = 0;
					select = true;
				}

				if (inSelect == 4)
				{
					board->MoveCards();
					inSelect = 0;
					select = true;
				}
			}
		}
	}

	return exit;
}