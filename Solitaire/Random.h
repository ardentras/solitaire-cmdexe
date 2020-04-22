#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
using std::srand; 
using std::rand; 
#include <ctime>
using std::time;

/************************************************************************
* Class: Random
*
* Purpose: A class for implementing RNG.
*
* Manager functions:
*		Random()
*			Seeds rand with the current time at instantiation.
* Methods:
*		GetRandom(int n)
*			Returns a random number between 0 and n.
*
*************************************************************************/

class Random
{
public:  
	Random() { srand(time(0)); }

	int GetRandom(int n) const { return rand() % n; }
};
#endif