/*************************************************************
* Author:			Shaun Rasmusen
* Filename:			Board.cpp
* Date Created:		5/17/2016
* Modifications:	None.
************************************************************/
#include "Board.h"

#include <iostream>
#include <Windows.h>

const char * logo =
"|  __  | "
"| /__` | "
"| .__/ | "
"|  __  | "
"| /  \\ | "
"| \\__/ | "
"|      | "
"| |    | "
"| |___ | "
"|      | "
"|  ()  | "
"|  ||  | "
"| ____ | "
"|  ||  | "
"|  ||  | "
"|      | "
"|  /\\  | "
"| /~~\\ | "
"|      | "
"|  ()  | "
"|  ||  | "
"|  __  | "
"| |__) | "
"| |  \\ | "
"|  ___ | "
"| |__  | "
"| |___ |\0";

/************************************************************************
* Purpose: One-arg constructor for the Board class.
*
* Precondition:
*		deck - A Deck object.
*
* Postcondition:
*		Sets m_deck to deck, initializes m_current_card and voidCard. The
*		console window is resized and positioned and InitBoard() is 
*		called.
*
************************************************************************/

Board::Board(Deck deck) : m_deck(deck), m_current_card(24), voidCard(Card(RERR, SERR, 2))
{
	HANDLE cHandle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(cHandle, ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);

	HWND console = GetConsoleWindow();
	
	int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
	int monitorHeight = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(console, monitorWidth / 2 - 300, monitorHeight / 2 - 400, 600, 800, TRUE);

	m_suits.resize(4);
	for (int i = 0; i < 4; i++)
	{
		m_suits[i].push_back(voidCard);
	}
	InitBoard();
}

/************************************************************************
* Purpose: Initializes the main elements on the board.
*
* Precondition:
*		None.
*
* Postcondition:
*		Shuffles the deck and deals out cards to m_draw and m_stacks.
*		Draws the initial elements of the Board to the console window.
*
************************************************************************/

void Board::InitBoard()
{
	m_start = false;
	m_score = 0;
	time(&m_time);
	m_deck.Shuffle();
	m_stacks.resize(7);

	// Deals out cards to the stacks.
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			m_stacks[i].push_back(m_deck.Deal());
			if (j == i)
				m_stacks[i][j].SetSide(0);
			else
				m_stacks[i][j].SetSide(1);
		}
		DrawStacks(i);
	}

	// Deals the remaining cards to the draw.
	for (int i = 0; i < 24; i++)
	{
		m_draw.push_back(m_deck.Deal());
		m_draw[i].SetSide(1);
	}

	DrawDraw();
	DrawSuits();

	cursor.X = 45;
	cursor.Y = 0;
	WORD logoAttrib[8];
	memset(logoAttrib, CYAN_BLACK, 8 * sizeof(WORD));

	for (int i = 0; i < 27; i++)
	{
		WriteConsoleOutputAttribute(hStdout, logoAttrib, 8, cursor, &numWrit);
		WriteConsoleOutputCharacterA(hStdout, &logo[i * 9], 9, cursor, &numWrit);
		cursor.Y++;
	}

	cursor.X = 45;
	cursor.Y = 31;

	WriteConsoleOutputCharacterA(hStdout, "+------+", 8, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputCharacterA(hStdout, "|Replay|", 8, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputCharacterA(hStdout, "+------+", 8, cursor, &numWrit);

	cursor.Y += 2;

	WriteConsoleOutputCharacterA(hStdout, "+------+", 8, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputCharacterA(hStdout, "| Exit |", 8, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputCharacterA(hStdout, "+------+", 8, cursor, &numWrit);

	cursor.Y += 2;

	WriteConsoleOutputCharacterA(hStdout, "Shaun", 8, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputCharacterA(hStdout, "Rasmusen", 8, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputCharacterA(hStdout, "(c) 2016", 8, cursor, &numWrit);
}

/************************************************************************
* Purpose: Displays a single card to the console.
*
* Precondition:
*		c - A Card object to be drawn.
*
* Postcondition:
*		Prints a Card object to the console in the form of:
*			?????
*			?R  ?
*			? S ?
*			?  R?
*			?????
*		Will print differently based on the type attribute of the Card.
*
************************************************************************/

void Board::DrawCard(Card c)
{
	buffer[0] = DTOP_LEFT_CORNER;
	for (int i = 1; i < 4; i++)
		buffer[i] = DHORIZ;
	buffer[4] = DTOP_RIGHT_CORNER;

	for (int i = 1; i < 4; i++)
	{
		buffer[i * 5] = DVERT;
		for (int j = 1; j < 4; j++)
			buffer[i * 5 + j] = ' ';
		buffer[i * 5 + 4] = DVERT;
	}

	buffer[4 * 5] = DBOTTOM_LEFT_CORNER;
	for (int i = 1; i < 4; i++)
		buffer[4 * 5 + i] = DHORIZ;
	buffer[4 * 5 + 4] = DBOTTOM_RIGHT_CORNER;

	if (c.GetSide() == 0 || c.GetSide() == 3)			// Face up/selected
	{
		// Set top rank
		if (c.GetRank() == ACE)
			buffer[1 * 5 + 1] = 'A';
		else if (c.GetRank() == JACK)
			buffer[1 * 5 + 1] = 'J';
		else if (c.GetRank() == QUEEN)
			buffer[1 * 5 + 1] = 'Q';
		else if (c.GetRank() == KING)
			buffer[1 * 5 + 1] = 'K';
		else if (c.GetRank() == RERR)
			buffer[1 * 5 + 1] = '!';
		else
			_itoa(c.GetRank(), &buffer[1 * 5 + 1], 10);

		// Set suit character
		buffer[2 * 5 + 2] = static_cast<char>(c.GetSuit() + 3);

		// Set bottom rank
		if (c.GetRank() != TEN)
			buffer[3 * 5 + 3] = buffer[1 * 5 + 1];
		else
		{
			buffer[3 * 5 + 2] = '1';
			buffer[3 * 5 + 3] = '0';
		}

		if (c.GetSide() != 3)
		{
			memset(attrib, DBLUE_WHITE, 25 * sizeof(WORD));
			if (c.GetSuit() == DIAMONDS || c.GetSuit() == HEARTS)
			{
				for (int i = 1; i < 4; i++)
					memset(&attrib[i * 5 + 1], RED_WHITE, 3 * sizeof(WORD));
			}
			else
			{
				for (int i = 1; i < 4; i++)
					memset(&attrib[i * 5 + 1], BLACK_WHITE, 3 * sizeof(WORD));
			}
		}
		else
		{
			memset(attrib, WHITE_BLACK, 25 * sizeof(WORD));
			if (c.GetSuit() == DIAMONDS || c.GetSuit() == HEARTS)
			{
				for (int i = 1; i < 4; i++)
					memset(&attrib[i * 5 + 1], CYAN_BLACK, 3 * sizeof(WORD));
			}
		}
	}
	else if (c.GetSide() == 1)			// Face down
	{
		memset(attrib, WHITE_RED, 25 * sizeof(WORD));
	}
	else if (c.GetSide() == 2)			// No card
	{
		buffer[0] = TOP_LEFT_CORNER;
		for (int i = 1; i < 4; i++)
			buffer[i] = HORIZ;
		buffer[4] = TOP_RIGHT_CORNER;

		for (int i = 1; i < 4; i++)
		{
			buffer[i * 5] = VERT;
			for (int j = 1; j < 4; j++)
				buffer[i * 5 + j] = ' ';
			buffer[i * 5 + 4] = VERT;
		}

		buffer[4 * 5] = BOTTOM_LEFT_CORNER;
		for (int i = 1; i < 4; i++)
			buffer[4 * 5 + i] = HORIZ;
		buffer[4 * 5 + 4] = BOTTOM_RIGHT_CORNER;

		memset(attrib, WHITE_BLACK, 25 * sizeof(WORD));
	}

	// Draw the card
	for (int i = 0; i < 5; i++)
	{
		WriteConsoleOutputAttribute(hStdout, &attrib[i * 5], 5, cursor, &numWrit);
		WriteConsoleOutputCharacterA(hStdout, &buffer[i * 5], 5, cursor, &numWrit);
		cursor.Y++;
	}

	cursor.Y -= 5;
}

/************************************************************************
* Purpose: Draws the stacks section of the board.
*
* Precondition:
*		x - The index of the stack to be drawn.
*
* Postcondition:
*		Draws a stack of cards to the console.
*
************************************************************************/

void Board::DrawStacks(int x)
{
	memset(attrib, WHITE_BLACK, 6 * sizeof(WORD));

	memset(buffer, ' ', 6 * sizeof(char));
	cursor.X = x * 6;
	cursor.Y = 6;

	for (int i = 0; i < 30; i++)
	{
		WriteConsoleOutputAttribute(hStdout, attrib, 6, cursor, &numWrit);
		WriteConsoleOutputCharacterA(hStdout, buffer, 6, cursor, &numWrit);
		cursor.Y++;
	}

	int faceup = 0;
	for (int j = 0; j < m_stacks[x].size(); j++)
	{
		cursor.X = (x * 6) + 1;
		cursor.Y = j + 6;
		
		if (m_stacks[x][j].GetSide() == 0 || m_stacks[x][j].GetSide() == 3)
			cursor.Y += faceup++;
		DrawCard(m_stacks[x][j]);
	}
}

/************************************************************************
* Purpose: Draws the draw pile to the console.
*
* Precondition:
*		None.
*
* Postcondition:
*		Draws the draw pile to the console.
*
************************************************************************/

void Board::DrawDraw()
{
	memset(attrib, WHITE_BLACK, 9 * sizeof(WORD));

	memset(buffer, ' ', 9 * sizeof(char));

	cursor.X = 27;
	cursor.Y = 0;

	for (int i = 0; i < 6; i++)
	{
		WriteConsoleOutputAttribute(hStdout, attrib, 9, cursor, &numWrit);
		WriteConsoleOutputCharacterA(hStdout, buffer, 9, cursor, &numWrit);
		cursor.Y++;
	}

	cursor.X = 27;
	cursor.Y = 0;

	if (m_draw.size() - m_current_card < 3)
	{
		for (int i = 0; i < 3 - (m_draw.size() - m_current_card); i++)
		{
			DrawCard(voidCard);
			cursor.X += 2;
		}
		for (int i = m_draw.size() - m_current_card; i > 0; i--)
		{
			DrawCard(m_draw[m_current_card + i - 1]);
			cursor.X += 2;
		}
	}
	else
	{
		for (int i = 2; i >= 0; i--)
		{
			DrawCard(m_draw[m_current_card + i]);
			cursor.X += 2;
		}
	}

	cursor.X = 37;
	cursor.Y = 0;

	if (m_current_card != 0)
		DrawCard(m_draw[m_current_card - 1]);
	else
		DrawCard(voidCard);
}

/************************************************************************
* Purpose: Draws the suits section of the board.
*
* Precondition:
*		None.
*
* Postcondition:
*		Draws the top card of any working suit to the console.
*
************************************************************************/

void Board::DrawSuits()
{
	cursor.X = 1;
	cursor.Y = 0;

	for (int i = 0; i < 4; i++)
	{
		DrawCard(m_suits[i].back());

		// If void card, will fill center space with suit.
		cursor.X += 2;
		cursor.Y += 2;

		buffer[0] = static_cast<char>(i + 3);
		WriteConsoleOutputCharacterA(hStdout, buffer, 1, cursor, &numWrit);

		cursor.X += 4;
		cursor.Y -= 2;
	}
}

/************************************************************************
* Purpose: Finds the location (if valid) of the card selected by the
*			cursor.
*
* Precondition:
*		x - The x coordinate of the cursor.
*		y - The y coordinate of the cursor.
*
* Postcondition:
*		Returns a value corresponding to the location of the selected
*		card (0 - stacks; 1 - draw; 2 - suits).
*
************************************************************************/

int Board::FindCard(int x, int y)	// 0 - stacks; 1 - draw; 2 - suits
{
	if (y < 5)
	{
		if (x >= 0 && x < 4)
			return 2;

		if (x == 5)
			return 1;
	}
	else if (y > 5)
	{
		y -= 6;
		if (x < 7)
		{ 
			int height = (m_stacks[x].size() - 1 + 5); // "- 1" to zero-base, "+ 5" for height of card.
			
			int facedown = -1;
			while (m_stacks[x][++facedown].GetSide() == 1);

			height += m_stacks[x].size() - facedown - 1;	// + 1 for every face-up card.

			if (y < height)
				return 0;
		}
	}

	return -1;
}

/************************************************************************
* Purpose: If FindCard() returns a valid location, temp is filled with
*			the cards pertaining to the selection.
*
* Precondition:
*		None.
*
* Postcondition:
*		Returns true if the card selected is a valid choice and fills
*		temp with the relevant cards.
*
************************************************************************/

bool Board::GetFrom()
{
	temp.clear();

	int facedown = -1, faceup = 0;
	int fromX = m_select[0];
	int fromY = m_select[1];
	int fromArr = FindCard(fromX / 6, m_select[1]);

	bool isValid = true;
	
	if (fromX % 6 != 0 && fromArr != -1)
	{
		fromX /= 6;
		switch (fromArr)
		{
		case 0:
			fromY -= 6;			// Zero-bases y from the top of the stack.

			while (m_stacks[fromX][++facedown].GetSide() == 1); // Finds total # of facedown cards.
			faceup = m_stacks[fromX].size() - facedown - 1;

			if (fromY > m_stacks[fromX].size() + faceup)
				fromY = m_stacks[fromX].size() + faceup;

			if (m_stacks[fromX][(fromY - facedown - 1) / 2 + facedown].GetSide() == 0)	// If face-up...
			{
				for (int i = (fromY - facedown) / 2 + facedown; i < m_stacks[fromX].size(); i++)
				{
					temp.push_back(m_stacks[fromX][i]);
					m_stacks[fromX][i].SetSide(3);
				}
				DrawStacks(fromX);
			}
			else
				isValid = false;
			break;
		case 1:
			if (m_draw.size() != 0 && m_draw.size() != m_current_card)
			{
				temp.push_back(m_draw[m_current_card]);
				m_draw[m_current_card].SetSide(3);
				DrawDraw();
				m_draw[m_current_card].SetSide(0);
			}
			else
				isValid = false;
			break;
		case 2:
			if (m_suits[fromX].back().GetSide() != 2)
			{
				temp.push_back(m_suits[fromX].back());
				m_suits[fromX].back().SetSide(3);
				DrawSuits();
			}
			else
				isValid = false;
		}
	}
	else
		isValid = false;

	return isValid;
}

/************************************************************************
* Purpose: Moves a card to the stacks.
*
* Precondition:
*		None.
*
* Postcondition:
*		Adds the contents of temp to the top of stacks.
*
************************************************************************/

void Board::MoveToStacks()
{
	if (m_stacks[m_select[2]].back().GetSide() == 2)
		m_stacks[m_select[2]].pop_back();

	for (int i = 0; i < temp.size(); i++)
		m_stacks[m_select[2]].push_back(temp[i]);
}

/************************************************************************
* Purpose: Resizes the stack after moving cards from it.
*
* Precondition:
*		stack - The index of the stack to resize.
*		size - The number of cards to resize down.
*
* Postcondition:
*		Resizes the chosen stack to remove moved cards.
*
************************************************************************/

void Board::ResizeStacks(int stack, int size)
{
	for (int i = 0; i < size; i++)
		m_stacks[stack].pop_back();

	if (m_stacks[stack].size() == 0)
		m_stacks[stack].push_back(voidCard);
	else if (m_stacks[stack].back().GetSide() == 1)
	{
		m_stacks[stack].back().SetSide(0);
		m_score += 5;
	}
}

/************************************************************************
* Purpose: Moves cards from one location to another.
*
* Precondition:
*		None.
*
* Postcondition:
*		Moves cards from one valid location to another, resizing and 
*		adjusting as necessary.
*
************************************************************************/

void Board::MoveCards()
{
	if (!m_start)
		m_start = true;

	if (temp.size() == 0)
		temp.resize(1);

	int fromArr = FindCard(m_select[0] / 6, m_select[1]);
	int toArr = FindCard(m_select[2] / 6, m_select[3]);

	if (m_select[2] % 6 != 0 && toArr != -1)
	{
		m_select[0] /= 6;
		m_select[2] /= 6;
		if (toArr == 0)			// To stacks...
		{
			// Because it will always go to the top card of a stack.
			m_select[3] = m_stacks[m_select[2]].size() - 1;

			if (temp.size() == 1 && m_stacks[m_select[2]][m_select[3]] == temp[0])
			{
				if (m_suits[temp[0].GetSuit()].back().GetRank() + 1 == temp[0].GetRank()
					|| (m_suits[temp[0].GetSuit()].back().GetSide() == 2 
						&& temp[0].GetRank() == ACE))
				{
					m_suits[temp[0].GetSuit()].push_back(temp[0]);

					ResizeStacks(m_select[0], temp.size());

					DrawStacks(m_select[0]);
				}
			}
			else if ((m_stacks[m_select[2]][m_select[3]].GetSuit() < 2 && temp[0].GetSuit() > 1)
				|| (m_stacks[m_select[2]][m_select[3]].GetSuit() > 1 && temp[0].GetSuit() < 2)
				|| (m_stacks[m_select[2]].back().GetSide() == 2))
			{
				if (m_stacks[m_select[2]][m_select[3]].GetRank() - 1 == temp[0].GetRank()
					|| (m_stacks[m_select[2]].back().GetSide() == 2 && temp[0].GetRank() == KING))
				{
					MoveToStacks();
					if (fromArr == 0)				// From stack to stack
					{
						ResizeStacks(m_select[0], temp.size());

						DrawStacks(m_select[0]);
					}
					else if (fromArr == 1)			// From draw to stack
					{
						m_draw.erase(m_draw.begin() + m_current_card);

						m_score += 5;

						DrawDraw();
					}
					else if (fromArr == 2)			// From suits to stack
					{
						m_suits[m_select[0]].pop_back();

						m_score -= 15;

						if (m_score < 0)
							m_score = 0;

						DrawSuits();
					}
					DrawStacks(m_select[2]);
				}
			}
		}
		else if (toArr == 1)
		{
			if (temp.size() == 1 && m_draw[m_current_card] == temp[0])
			{
				if (m_suits[temp[0].GetSuit()].back().GetRank() + 1 == temp[0].GetRank()
					|| (m_suits[temp[0].GetSuit()].back().GetSide() == 2
						&& temp[0].GetRank() == ACE))
				{
					m_suits[temp[0].GetSuit()].push_back(temp[0]);

					m_draw.erase(m_draw.begin() + m_current_card);

					m_score += 10;

					DrawDraw();
					DrawSuits();
				}
			}
		}
		else if (toArr == 2)		// To suits...
		{
			if (m_select[2] == temp[0].GetSuit())
			{
				if (m_suits[m_select[2]].back().GetRank() + 1 == temp[0].GetRank()
					|| (m_suits[m_select[2]].back().GetSide() == 2
						&& temp[0].GetRank() == ACE))
				{
					if (fromArr == 0)			// From stack to suits
					{
						m_suits[m_select[2]].push_back(temp[0]);

						ResizeStacks(m_select[0], temp.size());

						m_score += 10;

						DrawStacks(m_select[0]);
					}
					if (fromArr == 1)			// From draw to suits
					{
						m_suits[m_select[2]].push_back(temp[0]);

						m_draw.erase(m_draw.begin() + m_current_card);

						m_score += 10;

						DrawDraw();
					}

					DrawSuits();
				}
			}
		}		// endif toArr
	}		// endif fromArr

	ClearSelect();

	CheckWin();
}

/************************************************************************
* Purpose: Clears the selection by resetting the cards type to 0.
*
* Precondition:
*		None.
*
* Postcondition:
*		Resets any card with type 2 to type 0.
*
************************************************************************/

void Board::ClearSelect()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < m_stacks[i].size(); j++)
		{
			if (m_stacks[i][j].GetSide() == 3)
				m_stacks[i][j].SetSide(0);
		}
		DrawStacks(i);
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_suits[i].back().GetSide() == 3)
			m_suits[i].back().SetSide(0);
	}
	DrawSuits();

	if (m_draw.size() != 0)
	{
		if (m_current_card != m_draw.size())
		{
			if (m_draw[m_current_card].GetSide() == 3)
				m_draw[m_current_card].SetSide(0);
		}
	}
	DrawDraw();
}

/************************************************************************
* Purpose: Checks if the player has won the game.
*
* Precondition:
*		None.
*
* Postcondition:
*		Checks if the board is cleared or eligible for autocompletion.
*
************************************************************************/

void Board::CheckWin()
{
	bool autocomplete = true;

	if (m_draw.size() == 0)
	{
		m_win = true;
		for (int i = 0; i < 7; i++)
		{
			if (m_stacks[i].back().GetSide() != 2)
				m_win = false;
			if (m_stacks[i].front().GetSide() != 0 && m_stacks[i].front().GetSide() != 2)
				autocomplete = false;
		}
	}
	else
		autocomplete = false;

	if (!m_win && autocomplete)
	{
		AutoComplete();
	}
}

/************************************************************************
* Purpose: Autocompletes the board.
*
* Precondition:
*		None.
*
* Postcondition:
*		Moves all cards from stacks to the suits, completing the game.
*
************************************************************************/

void Board::AutoComplete()
{
	for (int i = 1; i < 14; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (m_stacks[j].back().GetRank() == i)
			{
				m_suits[m_stacks[j].back().GetSuit()].push_back(m_stacks[j].back());

				m_score += 5;

				ResizeStacks(j, 1);

				DrawStacks(j);
				DrawSuits();
			}
		}
	}

	m_win = true;
}

/************************************************************************
* Purpose: Prints out information pertaining to a winning state.
*
* Precondition:
*		None.
*
* Postcondition:
*		Displays winning message and score to the console.
*
************************************************************************/

void Board::DisplayWin()
{
	time_t currTime;
	time(&currTime);

	// Deduction for time to finish.
	// (Time to finish) * (2pts/5sec)
	m_score -= ((currTime - m_time) * .4);

	// Bonus for completing the game quickly.
	// If time < 30, bonus = 5000
	// Else bonus = 5000 - (((time to complete) - 30) * 5)
	m_score += ((currTime - m_time) > 30) ? 
				5000 - (((currTime - m_time) - 30) * 25) : 5000;
	cursor.X = 2;
	cursor.Y = 7;

	WORD winAttrib[22];
	memset(winAttrib, RED_WHITE, 22 * sizeof(WORD));

	char content[22];
	content[0] = DTOP_LEFT_CORNER;
	for (int i = 1; i < 20; i++)
		content[i] = DHORIZ;
	content[20] = DTOP_RIGHT_CORNER;

	WriteConsoleOutputAttribute(hStdout, winAttrib, 21, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, content, 21, cursor, &numWrit);

	memset(winAttrib, BLACK_WHITE, 22 * sizeof(WORD));
	winAttrib[0] = RED_WHITE;
	winAttrib[20] = RED_WHITE;

	cursor.Y++;
	strcpy(content, "   CONGRATULATIONS   ");
	content[0] = DVERT;
	content[1] = static_cast<char>(SPADES + 3);
	content[19] = static_cast<char>(SPADES + 3);
	content[20] = DVERT;

	WriteConsoleOutputAttribute(hStdout, winAttrib, 21, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, content, 21, cursor, &numWrit);

	memset(winAttrib, RED_WHITE, 22 * sizeof(WORD));

	cursor.Y++;
	content[0] = DBOTTOM_LEFT_CORNER;
	for (int i = 1; i < 20; i++)
		content[i] = DHORIZ;
	content[20] = DBOTTOM_RIGHT_CORNER;

	WriteConsoleOutputAttribute(hStdout, winAttrib, 21, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, content, 21, cursor, &numWrit);
	cursor.Y += 3;
	cursor.X += 5;

	memset(winAttrib, WHITE_BLACK, 22 * sizeof(WORD));
	WriteConsoleOutputAttribute(hStdout, winAttrib, 12, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, "You've won!", 12, cursor, &numWrit);

	cursor.Y += 2;
	WriteConsoleOutputAttribute(hStdout, winAttrib, 12, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, "---Score---", 12, cursor, &numWrit);

	char buffer[16];
	itoa(m_score, buffer, 10);
	cursor.Y += 1;
	cursor.X += 5 - (strlen(buffer) / 2);

	WriteConsoleOutputAttribute(hStdout, winAttrib, strlen(buffer), cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, buffer, strlen(buffer), cursor, &numWrit);

	cursor.Y += 2;
	cursor.X -= 10 - (strlen(buffer) / 2);

	WriteConsoleOutputAttribute(hStdout, winAttrib, 22, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, "If you'd like to play", 22, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputAttribute(hStdout, winAttrib, 21, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, "again, press replay,", 21, cursor, &numWrit);
	cursor.Y++;
	WriteConsoleOutputAttribute(hStdout, winAttrib, 22, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, "or else press exit to", 22, cursor, &numWrit);
	cursor.Y++;
	cursor.X += 8;
	WriteConsoleOutputAttribute(hStdout, winAttrib, 5, cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, "quit.", 5, cursor, &numWrit);
}

/************************************************************************
* Purpose: Displays a timer showing elapsed time during the current game.
*
* Precondition:
*		None.
*
* Postcondition:
*		Outputs a timer to the console in the game's sidebar.
*
************************************************************************/

void Board::DisplayTime()
{
	if (!m_start)
		time(&m_time);

	time_t currTime;
	time(&currTime);

	char buffer[16];

	cursor.X = 47;
	cursor.Y = 28;

	WriteConsoleOutputCharacterA(hStdout, "Time", 5, cursor, &numWrit);
	
	char otherBuffer[3];
	itoa((int)((currTime - m_time) / 60), buffer, 10);
	strcat(buffer, ((currTime - m_time) % 60) < 10 ? ":0" : ":");
	itoa((int)(currTime - m_time) % 60, otherBuffer, 10);
	strcat(buffer, otherBuffer);

	cursor.X -= strlen(buffer) / 2 - 2;
	cursor.Y++;
	
	WriteConsoleOutputCharacterA(hStdout, "         ", strlen(buffer), cursor, &numWrit);
	WriteConsoleOutputCharacterA(hStdout, buffer, strlen(buffer), cursor, &numWrit);
}

/************************************************************************
* Purpose: Deals out cards from the draw pile.
*
* Precondition:
*		None.
*
* Postcondition:
*		Deals out three cards from the draw pile.
*
************************************************************************/

void Board::Deal()
{
	if (!m_start)
		m_start = true;

	if (m_draw.size() != 0)
	{
		int toDeal = 3;

		if (m_draw.size() < 3)
			toDeal = m_current_card;
		else if (m_current_card < 3 && m_current_card > 0)
			m_current_card = 3;

		if (m_current_card == 0)
		{
			for (int i = 0; i < m_draw.size(); i++)
				m_draw[i].SetSide(1);

			m_current_card = m_draw.size();
		}
		else
		{
			for (int i = 0; i < toDeal; i++)
			{
				m_current_card--;
				m_draw[m_current_card].SetSide(0);
			}
		}

		DrawDraw();
	}
}