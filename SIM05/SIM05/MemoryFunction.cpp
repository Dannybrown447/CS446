// Program Information /////////////////////////////////////////////////////////
/**
 * @file SimulatorFunctions.c
 *
 * @brief Memory Address Locator Implementations for CS 446/646 at the 
 *		  Univserity of Nevada, Reno
 * 
 * @details This file includes the necessary function implementations for 
 *			assignment 4 and 5 of the CS 446/646 course simulator.
 *
 * @author Daniel Brown
 *
 * @version 1.00 (23 Feb 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef MEM_FUNC_CPP
#define MEM_FUNC_CPP

// HEADER FILES ////////////////////////////////////////////////////////////////

#include "MemoryFunction.h"

// GLOBAL CONSTANTS ////////////////////////////////////////////////////////////

 // None

/**
 * @brief Memory Adress Return Function
 *
 * @details Reads in an integer representing kilobytes of total memory. Using
 *			the total memory, the function returns the address as a hexadecimal
 *			string.
 *          
 * @param in: total memory in kb (int)
 *
 * @pre None
 *
 * @post Hexadecimal string address returned
 *
 * @exception Requires value > 0
 *
 * @exception Address pointer should be null
 */
unsigned int allocateMemory(int totMem, int memBlock, unsigned int currentAddr )
{
	//retrun value
	unsigned int address;
	//if the current address is grater than the total memory, it resets to zero
	//if the there isnt enough space to allocate an entire memory block it rolls back to zero
	if((currentAddr > totMem) || (currentAddr + memBlock) > totMem)
	{
		address = 0;
		return address;
	}
	else
	{
		return currentAddr;
	}
	
}

#endif // MEM_FUNC_C
