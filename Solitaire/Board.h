#ifndef _BOARD_H
#define _BOARD_H

#define TOP_LEFT_CORNER (char)218
#define TOP_RIGHT_CORNER (char)191
#define BOTTOM_LEFT_CORNER (char)192
#define BOTTOM_RIGHT_CORNER (char)217
#define HORIZ (char)196
#define VERT (char)179

#define DTOP_LEFT_CORNER (char)201
#define DTOP_RIGHT_CORNER (char)187
#define DBOTTOM_LEFT_CORNER (char)200
#define DBOTTOM_RIGHT_CORNER (char)188
#define DHORIZ (char)205
#define DVERT (char)186

#define DBLUE_WHITE (FOREGROUND_BLUE | BACKGROUND_RED \
					| BACKGROUND_GREEN | BACKGROUND_BLUE \
					| BACKGROUND_INTENSITY)
#define CYAN_BLACK (FOREGROUND_BLUE | FOREGROUND_GREEN \
					| FOREGROUND_INTENSITY)
#define RED_WHITE (FOREGROUND_RED | FOREGROUND_INTENSITY \
					| BACKGROUND_RED | BACKGROUND_GREEN \
					| BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define WHITE_RED (FOREGROUND_RED | FOREGROUND_GREEN \
					| FOREGROUND_BLUE | FOREGROUND_INTENSITY\
					| BACKGROUND_RED | BACKGROUND_INTENSITY)
#define BLACK_WHITE (BACKGROUND_RED | BACKGROUND_GREEN \
					| BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define WHITE_BLACK (FOREGROUND_RED | FOREGROUND_GREEN \
					| FOREGROUND_BLUE | FOREGROUND_INTENSITY)

#include <windows.h>
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <ctime>
using std::time;

#include "Deck.h"

/************************************************************************
* Class: 
*
* Purpose: 
*
* Manager functions:
*		Board(Deck deck)
*
* Methods:
*		InitBoard()
*			Initializes the sections of the board and draws initial setup.
*
*		DrawCard(Card c)
*			Draws a card.
*		DrawStacks(int x)
*			Draws a stack.
*		DrawDraw()
*			Draws the draw pile.
*		DrawSuits()
*			Draws the suits.
*	
*		FindCard(int x, int y)
*			Determines if a cursor selection is valid.
*		GetFrom()
*			Gets the cards in a selection and stores them in temp.
*		MoveToStacks()
*			Moves cards to another stack.
*		ResizeStacks(int stack, int size)
*			Resizes a stack down once cards are removed.
*		MoveCards()
*			Moves cards from one location to another.
*		ClearSelect()
*			Resets card types from 2 to 0, clearing a selection.
*		CheckWin()
*			Checks if the stacks and draw pile are clear.
*		AutoComplete()
*			Auto-completes the game if all cards are turned over and
*			draw is empty.
*		DisplayWin()
*			Display winning information, including score.
*		DisplayTime()
*			Displays a timer since the game start.
*	
*		Deal()
*			Deals out cards one at a time. Reshuffles when >52 dealt.
*	
*		SetSelect(int select, int val)
*			Sets the coordinates of the current selections.
*		GetWin()
*			Returns the value of m_win.
*		SetWin(bool win)
*			Sets m_win to win.
*
*************************************************************************/

class Board
{
private:
	COORD cursor;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD numWrit = 0;

	WORD attrib[42];
	char buffer[42];

	int m_score;
	time_t m_time;
	bool m_start;

	vector<vector<Card>> m_suits;
	vector<vector<Card>> m_stacks;
	vector<Card> m_draw;

	vector<Card> temp;
	const Card voidCard;

	Deck m_deck;

	int m_current_card;
	int m_select[4];	// 0/2 - x coords from & to; 1/3 - y coords from & to.

	bool m_win = false;
public:
	Board(Deck deck);

	void InitBoard();

	void DrawCard(Card c);

	void DrawStacks(int x);
	void DrawDraw();
	void DrawSuits();

	int FindCard(int x, int y);
	bool GetFrom();
	void MoveToStacks();
	void ResizeStacks(int stack, int size);
	void MoveCards();
	void ClearSelect();
	void CheckWin();
	void AutoComplete();
	void DisplayWin();
	void DisplayTime();

	void Deal();

	void SetSelect(int select, int val) { m_select[select] = val; }

	bool GetWin() const { return m_win; }
	void SetWin(bool win) { m_win = win; }
};

#endif // _BOARD_H