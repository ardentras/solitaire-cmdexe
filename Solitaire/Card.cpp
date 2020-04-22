/*************************************************************
* Author:			Shaun Rasmusen
* Filename:			Card.cpp
* Date Created:		4/24/2016
* Modifications:	5/7/2016 - Added m_type + mutators.
					5/17/2016 - Overloaded operator ==.
************************************************************/

#include <iostream>
using std::cout;
using std::endl;
#include <windows.h>

#include "Card.h"

/************************************************************************
* Purpose: Two arg constructor for the Card class.
*
* Precondition:
*		rank - A valid value from the Rank enum.
*		suit - A valid value from the Suit enum.
*
* Postcondition:
*		Sets m_rank to rank and m_suit to suit.
*
************************************************************************/

Card::Card(Rank rank, Suit suit, int type) : m_rank(rank), m_suit(suit), m_type(type)
{
}

/************************************************************************
* Purpose: Operator == overload for the Card class.
*
* Precondition:
*		rhs - The right-hand side of the operation.
*
* Postcondition:
*		Returns true if the rank and suit of the compared cards are equal.
*
************************************************************************/

bool Card::operator ==(const Card & rhs)
{
	return (m_rank == rhs.m_rank && m_suit == rhs.m_suit);
}