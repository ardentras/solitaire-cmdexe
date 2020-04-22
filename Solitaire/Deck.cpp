/*************************************************************
* Author:			Shaun Rasmusen
* Filename:			Deck.cpp
* Date Created:		4/24/2016
* Modifications:	None.
************************************************************/

#include "Deck.h"
#include "Card.h"
#include "Random.h"

Random Deck::rand;

/************************************************************************
* Purpose: Constructor for the Deck class.
*
* Precondition:
*		None.
*
* Postcondition:
*		Fills m_rank with all 52 valid playing cards and shuffles it.
*
************************************************************************/

Deck::Deck()
{
	for (int i = 0; i < 52; i++)
	{
		m_deck[i].SetRank(static_cast<Rank>((int)(i % 13) + 1));
		m_deck[i].SetSuit(static_cast<Suit>((int)(i / 13)));
	}

	Shuffle();

	m_current_card = 0;
}

/************************************************************************
* Purpose: Shuffles an array of Card objects using the Fisher-Yates
*			shuffle algorithm.
*
* Precondition:
*		None.
*
* Postcondition:
*		Randomizes the order of the objects within an array of Card 
*		objects.
*
************************************************************************/

void Deck::Shuffle()
{
	for (int i = 52; i > 0; i--)
	{
		int j = rand.GetRandom(52);
		
		Card temp = m_deck[j];
		m_deck[j].SetRank(m_deck[i - 1].GetRank());
		m_deck[j].SetSuit(m_deck[i - 1].GetSuit());
		m_deck[i - 1].SetRank(temp.GetRank());
		m_deck[i - 1].SetSuit(temp.GetSuit());
	}

	m_current_card = 0;
}

/************************************************************************
* Purpose: Deals out a single card when called.
*
* Precondition:
*		None.
*
* Postcondition:
*		Returns the element of m_deck at m_current_card. Increments
*		m_current_card by one.
*
************************************************************************/

Card Deck::Deal()
{
	if (m_current_card > 52)
		Shuffle();

	return m_deck[m_current_card++];
}