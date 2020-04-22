#ifndef DECK_H
#define DECK_H

#include "Card.h"

/************************************************************************
* Class: Deck
*
* Purpose: Holds an array of 52 Card objects that can be shuffled and
*			dealt.
*
* Manager functions:
*		Deck()
*			No parameters, fills the m_deck array and shuffles it.
*
* Methods:
*		Shuffle()
*			Shuffles m_deck using the Fisher-Yates shuffle algorithm.
*		Deal()
*			Returns the m_current_card element of m_deck and increments
*			m_current_card by one.
*
*************************************************************************/

class Random;

class Deck
{
private:
	Card m_deck[52];
	int m_current_card;

	static Random rand;
public:
	Deck();
	void Shuffle();
	Card Deal();
};

#endif