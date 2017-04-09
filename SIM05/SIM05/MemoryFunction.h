// Program Information /////////////////////////////////////////////////////////
/**
 * @file Simulator Functions.c
 *
 * @brief Memory Address Locator for CS 446/646 at the Univserity of 
 *        Nevada, Reno
 * 
 * @details This file includes the necessary function for assignment 2 of the 
 *          CS 446/646 course simulator.
 *
 * @author Vineeth Rajamohan
 *
 * @version 1.01 (27 Jan 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef MEM_FUNC_H
#define MEM_FUNC_H

// HEADER FILES ////////////////////////////////////////////////////////////////

#include <iostream>

// GLOBAL CONSTANTS ////////////////////////////////////////////////////////////

 // None

unsigned int allocateMemory(int totMem, int memBlock, unsigned int currentAddr );

#endif // MEM_FUNC_H
