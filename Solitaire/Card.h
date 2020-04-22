#ifndef CARD_H
#define CARD_H

enum Rank {ACE = 1, DEUCE, TREY, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, RERR};
enum Suit {HEARTS, DIAMONDS, CLUBS, SPADES, SERR};

/************************************************************************
* Class: Card
*
* Purpose: An object that holds the rank and suit of any one card an
*			provides the ability to print that card to the console.
*
* Manager functions:
*		Card(Rank rank = ACE, Suit suit = SPADES)
*			Two default parameters, sets m_rank & m_suit to rank & suit.
*
* Methods:
*		operator ==(const Card & rhs)
*			Returns true if the compared cards have same suit and rank.
*
*		SetRank(Rank rank)
*			Sets m_rank to rank.
*		SetSuit(Suit suit)
*			Sets m_suit to suit.
*		SetSide(int type)
*			Sets m_type to type.
*		GetRank()
*			Returns the value of m_rank.
*		GetSuit()
*			Returns the value of m_suit.
*		GetSide()
*			Returns the value of m_type.
*
*************************************************************************/

class Card
{
private:
	Rank m_rank;
	Suit m_suit;
	int m_type;
public:
	Card(Rank rank = ACE, Suit suit = SPADES, int type = 1);

	bool operator ==(const Card & rhs);
	
	void SetRank(Rank rank) { m_rank = rank; }
	void SetSuit(Suit suit) { m_suit = suit; }
	Rank GetRank() { return m_rank; }
	Suit GetSuit() { return m_suit; }

	int GetSide() { return m_type; }			// 0 - faceup; 1 - facedown; 2 - nocard
	void SetSide(int type) { m_type = type; }
};

#endif