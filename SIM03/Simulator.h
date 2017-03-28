// Program Information /////////////////////////////////////////////////////////
/**
 * @file Simulator.h
 *
 * @brief Simulator implemntattion file for project 2 at
 *		  Univserity of Nevada, Reno
 * 
 * @details This file includes the necessary function implementations for 
 *			assignment 2 of the CS 446/646 course simulator.
 *
 * @author Daniel Brown
 *
 * @version 1.01 (24 febuary 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef SIM_FUNC_H
#define SIM_FUNC_H





// HEADER FILES ////////////////////////////////////////////////////////////////
#include "ReadData.h"
#include "MemoryFunction.h"
#include <string>

struct threadData
{
	int m_time;
	std::string operation;
	PCB pcb;
	unsigned int address;
	int printerNum;
	int hardDriveNum[2];
};
class Simulator
{
private:
	
public:
/**
 * @brief Runs the simulator with the given files on the command line and in the config file
 *
 * @details Reads and organizes both the meta file and the config file and runs the simulation based on 
 *			the values given in the files
 *          
 * @param in: command line arguments
 *
 * @pre valid file names have been entered in the command line
 *
 * @post simulation is run and logged to monitor or file based on config file
 *
 */
	bool runSim(int argc, char *argv[]);

/**
 * @brief Prints the Data from the simulator
 *
 * @details Prints out the data based on the time vector that is being used in the runSim function
 *			that requires a vector templated to thread data
 *          
 * @param in: Vector templated to thread data
 *
 * @pre runSim has already been completed
 *
 * @post values are printed to either monitor or file as specified by the config file
 *
 */
	void printData(std::string word,double timeTaken, bool logToMon, bool logToFile, std::string outputFile, bool startCheck,unsigned int address);

/**
 * @brief Helps the print funcion with printing values from I/O operations
 *
 * @details compares the thread operation id to valid id's and prints 
 *			based on what data is held in the thread data's operation ID
 *          
 * @param in: Thread data item
 *				int i that holds the application number from amount of applications are run
 *
 * @pre runSim has already been completed
 *
 * @post values are printed to either monitor or file as specified by the config file
 *
 */
	void printStart(double timeTaken, int i, std::string word, bool logToMon, bool logToFile, std::string outputFile, threadData* toPass);
	void printEnd(double timeTaken, int i, std::string word, unsigned int address, bool logToMon, bool logToFile, std::string outputFile);
	
};

#endif