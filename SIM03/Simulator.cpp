// Program Information /////////////////////////////////////////////////////////
/**
 * @file Simulator.cpp
 *
 * @brief Simulator implemntattion file for project 2 at
 *		  Univserity of Nevada, Reno
 * 
 * @details This file includes the necessary function implementations for 
 *			assignment 2 of the CS 446/646 course simulator.
 *
 * @author Daniel Brown
 *
 * @version 3.00 (10 March 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef SIM_FUNC_CPP
#define SIM_FUNC_CPP


// HEADER FILES ////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "Simulator.h"
#include <time.h>  
#include <vector>
#include <pthread.h>
#include <fstream>
#include <iomanip>
#include <semaphore.h>
using namespace std::chrono ;
using std:: cout;
using std::endl;


//Global variables
const int New = 0;
const int ready = 1;
const int running = 2;
const int waiting = 3;
const int exiting = 4;
bool addrChange = false;
int hardQuan, printerQuan;
//mutex create
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
//create semaphore
sem_t printSem;
sem_t inHardDriveSem;
sem_t outHardDriveSem;


/**
 * @brief simulates the waiting time for a given amount of miliseconds
 *
 * @details uses system clock to wait for a given amount of time
 *          
 * @param in: int miliseconds the time to be waited for
 *
 * @pre 
 *
 * @post desired amount of time to wait has been completed
 *
 */
void wait(int milliseconds)
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}
/**
 * @brief Runs each I/O process using single threading
 *
 * @details function called by the created thread that simulates waiting for
 *			I/O operations to complete
 *          
 * @param in: void* militTime thread data struct that contains wait time
 *
 * @pre a thread has been created to run
 *
 * @post I/O process simulation is completed and time is returned via ADT
 *
 */
 void* runProcess(void* miliTime)
{
	
	threadData* data = (threadData*) miliTime;
	std::clock_t start;
	

	
    
    if(data->operation == "M(allocate")
    {
    	data->pcb.processState = running;
    	data->address = allocateMemory(data->pcb.systemMem, data->pcb.memBlock, data->address);
    	data->pcb.processState = waiting;
    	wait(data->m_time);
    	data->pcb.processState = exiting;   
    	addrChange = true;                                                                                                                                                
    }
    else if(data->operation == "O(printer")
    {
    	sem_wait(&printSem);
    	data->pcb.processState = running;
    	data->pcb.processState = waiting;
    	data->printerNum += 1;
    	wait(data->m_time);
    	data->pcb.processState = exiting;
    	addrChange = false;
    	sem_post(&printSem);

    	
    }

    else if(data->operation == "O(harddrive")
    {
    	sem_wait(&outHardDriveSem);
    	data->pcb.processState = running;
    	data->pcb.processState = waiting;
    	data->hardDriveNum[0] += 1;
    	wait(data->m_time);
    	data->pcb.processState = exiting;
    	addrChange = false;
    	sem_post(&outHardDriveSem);
    }
    else if(data->operation == "I(harddrive")
    {
    	
    	sem_wait(&inHardDriveSem);
    	data->pcb.processState = running;
    	data->pcb.processState = waiting;
    	data->hardDriveNum[1] += 1;
    	wait(data->m_time);
    	data->pcb.processState = exiting;
    	addrChange = false;
    	sem_post(&inHardDriveSem);
    }
    else
    {
    	pthread_mutex_lock (&mymutex);
    	data->pcb.processState = running;
    	data->pcb.processState = waiting;
    	wait(data->m_time);
    	data->pcb.processState = exiting;
    	addrChange = false;
    	pthread_mutex_unlock (&mymutex); 
    }
    //cout << std::fixed<< *duration << endl;
 	
    pthread_exit(data);
}//end runProcess

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
bool Simulator:: runSim(int argc, char *argv[])
{
	// VARIABLE DEFINITIONS  ///////////////////////////
	ReadData readData;
	bool logToMon = false;
	bool logToFile = false;
	std::vector<std::vector<cycleInfo>> vec(readData.getData(argc,argv));
	std::vector<cycleInfo> cycle (vec[0]);
	logToMon = readData.getLogMon();
	logToFile = readData.getLogFile();
	std::string outputFile = readData.getFileName();
	threadData *toPass = new threadData;
	threadData* newData = new threadData;
	int threadReturn = 0;
	int index = 0;
	int memBlockSize,systemMem;
	void* status;
	std::clock_t start;	
	double timeTaken;
	int counter = 0;
	int tempCheck = 0;


	



	/* go through the vector to get the quantity of hard drives and printers 
		and get memory information
	*/
	for(int i = 0; i < cycle.size(); i++)
	{
	   if(cycle[i].id == "Harddrive quantity")
	   {
	   	
	   	hardQuan = cycle[i].cycleTime;
	   }
	   else if(cycle[i].id == "Printer quantity")
	   {

	    printerQuan = cycle[i].cycleTime;
	   }
	  else if(cycle[i].id == "Memory block size")
	   {
	     memBlockSize = cycle[i].cycleTime;
	     
	   }
	   else if(cycle[i].id == "System memory")
	   {
	     systemMem = cycle[i].cycleTime;
	     
   	   }
	}
	if(systemMem < memBlockSize)
	{
		std::cerr << "Memory block size is greater than total system memory, ending simulator\n";
		exit(EXIT_FAILURE);
	}
	//initialize it to only available to this thread and printer/hardDrive quanity
	
	sem_init(&printSem, 0, printerQuan);
	sem_init(&inHardDriveSem, 0, hardQuan);
	sem_init(&outHardDriveSem, 0, hardQuan);

	//initalize values of the thread data 
	toPass->hardDriveNum[0] = 0;//output harddrive count
	toPass->hardDriveNum[1] = 0;//input harddrive count
	toPass->printerNum = 0;//number of printers used
	toPass->address = 0;
	toPass->pcb.systemMem = systemMem;
	toPass->pcb.memBlock = memBlockSize;
	
		if(cycle[0].id == "S(start" )
		{
			start = std::clock();
			//the ending condition is the length of the cycles vector minus four becuase of the printer/hard drive quantity and 
				//system memory and block size 
			while(index < cycle.size()-4)
			{
				//copying values to a struct to pass to the run process function
				toPass->m_time = cycle[index].cycleTime;
				toPass->operation = cycle[index].id;
				if(toPass->operation == "A(start")
					counter++;
				toPass->pcb.processState = cycle[index].pcb.processState;
				//start time of the operation
				timeTaken =  ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
				printStart(timeTaken,counter,cycle[index].id, logToMon, logToFile, outputFile, toPass);
				//pthread funtions
				pthread_t tid;//thread ID
				pthread_attr_t attr;//attributes
				pthread_attr_init(&attr);//default attributes
				threadReturn = pthread_create(&tid,&attr,runProcess, (void*) toPass);//creating the new thread
				pthread_join(tid,&status);//ending the thread and getting the return value
				//end of time operation
				timeTaken = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
				newData = (threadData*) status;//gets the address from the thread
				//checks if the memory allocate has been called 
				//	if it was then the value of toPass is changed to new value
				if(addrChange == true)
				{
					
					toPass->address = newData->address;//assign address from returned data
					addrChange = false;//change it back to false
					printEnd(timeTaken,counter,cycle[index].id, newData->address, logToMon, logToFile, outputFile);	//prints output	
					toPass->address += memBlockSize;//incriments address by memblock size
				}
				else
				{
					toPass->hardDriveNum[0] = newData->hardDriveNum[0];//sets the number of the output hard drive
					toPass->hardDriveNum[1] = newData->hardDriveNum[1];//sets the number of the input hard drive

					//checks if the pninter number is greater than the amount of printers in config file
					tempCheck = newData->printerNum;
					if(tempCheck >= printerQuan)
						toPass->printerNum = 0;
					//check that the output hardrive number is less than the total
					tempCheck = newData->hardDriveNum[0];
					if(tempCheck >= hardQuan)
						toPass->hardDriveNum[0] = 0;//sets to zero if it the number is higher than the total number

					//check that the input hardrive number is less than the total
					tempCheck = newData->hardDriveNum[1];
					if(tempCheck >= hardQuan)
						toPass->hardDriveNum[1] = 0;//sets to zero if it the number is higher than the total number

					//function call to print the end of each cycle
					printEnd(timeTaken,counter,cycle[index].id, newData->address, logToMon, logToFile, outputFile);			
				}
				
				index++;
				
			}
			
			timeTaken = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
			cout << timeTaken << " - Simulator program ending\n";
		}
		else
		{
			std::cerr << "No Operating system start, ending simulator " << endl;
		}
	//delete mutex and all semphores that were used
    pthread_mutex_destroy(&mymutex);
	sem_destroy(&printSem);
	sem_destroy(&inHardDriveSem);
	sem_destroy(&outHardDriveSem);
}


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
void Simulator:: printStart(double timeTaken, int i, std::string word, bool logToMon, bool logToFile, std::string outputFile, threadData* toPass)
{
	std::ofstream fout;

	if(logToMon == true)
	{
		if(word == "S(start")
		{
			cout << std::fixed<< timeTaken <<" - Simulator program starting\n";
		}
		else if(word == "A(start") 
		{
			cout << timeTaken<< " - OS: preparing process " << i << endl;
		}
		else if(word == "A(end")
		{
			//do nothing
		}
		else if(word == "M(allocate")
		{
			cout << timeTaken << " - Process " << i << ": allocating memory\n" ;
			
		}
		else if(word == "M(block")
		{
			cout << timeTaken  << " - Process " << i << ": start memory blocking\n";
		}
		else if(word == "P(run")
		{
			cout << timeTaken  << " - Process " << i << ": start processing action\n"; 
			
		}
		else if(word == "I(harddrive")
		{
			cout << timeTaken  << " - Process " << i << ": start harddrive input on HDD " << toPass->hardDriveNum[1] << endl;;  

		}
		else if(word == "O(harddrive")
		{
			cout << timeTaken  << " - Process " << i << ": start harddrive output on HDD " << toPass->hardDriveNum[0] << endl;
		}
		else if(word == "I(mouse")
		{
			cout << timeTaken  << " - Process " << i << ": start mouse input\n";
		}
		else if(word == "I(keyboard")
		{
			cout << timeTaken << " - Process " << i << ": start keyboard input\n";
		}
		else if(word == "O(monitor")
		{
			cout << timeTaken  << " - Process " << i << ": start monitor output\n";
		}
		else if(word == "O(speaker")
		{
			cout << timeTaken  << " - Process " << i << ": start speaker output\n";
		}
		else if(word == "O(printer")
		{
			cout << timeTaken  << " - Process " << i << ": start printer output on PRNTR " << toPass-> printerNum << endl;
		}
	}
	if(logToFile == true)
	{
		fout.open(outputFile, std::ios_base::app);
	    if (!fout.is_open())//tries to open files
	    { 
	    	std::cerr << "Could not open output file\n";
	    	exit(EXIT_FAILURE);
	   	}	
	  	else
	  	{
			if(word == "S(start")
			{
				fout << std::fixed<< timeTaken <<" - Simulator program starting\n";
			}
			else if(word == "A(start") 
			{
				fout << timeTaken<< " - OS: preparing process " << i << endl;
			}
			else if(word == "A(end")
			{
				//do nothing
			}
			else if(word == "M(allocate")
			{
				fout << timeTaken << " - Process " << i << ": allocating memory\n" ;
				
			}
			else if(word == "M(block")
			{
				fout << timeTaken  << " - Process " << i << ": start memory blocking\n";
			}
			else if(word == "P(run")
			{
				fout << timeTaken  << " - Process " << i << ": start processing action\n"; 
				
			}
			else if(word == "I(harddrive")
			{
				fout << timeTaken  << " - Process " << i << ": start harddrive input\n";  

			}
			else if(word == "O(harddrive")
			{
				fout << timeTaken  << " - Process " << i << ": start harddrive output\n";
			}
			else if(word == "I(mouse")
			{
				fout << timeTaken  << " - Process " << i << ": start mouse input\n";
			}
			else if(word == "I(keyboard")
			{
				fout << timeTaken << " - Process " << i << ": start keyboard input\n";
			}
			else if(word == "O(monitor")
			{
				fout << timeTaken  << " - Process " << i << ": start monitor output\n";
			}
			else if(word == "O(speaker")
			{
				fout << timeTaken  << " - Process " << i << ": start speaker output\n";
			}
			else if(word == "O(printer")
			{
				fout << timeTaken  << " - Process " << i << ": start printer output\n";
			}
		}


	}
	else
	{
		std::cerr<< "Not logged to file or monitor\n";
	}
}

void Simulator:: printEnd(double timeTaken, int i, std::string word, unsigned int address, bool logToMon, bool logToFile, std::string outputFile)
{
	std::ofstream fout;
	if(logToMon == true)
	{

		if(word == "S(start")
		{
			//do nothing
		}
		else if(word == "A(start") 
		{
			//do nothing
		}
		else if(word == "A(end")
		{
			cout << timeTaken << " - OS: Removing process " << i << endl;
		}
		else if(word == "M(allocate")
		{
			
			cout << timeTaken << " - process " << i << " memory allocated at 0x"<< std::hex << std::setw(8) << std::setfill('0') << address<< endl;
		}
		else if(word == "M(block")
		{
			cout << timeTaken << " - process " << i << ": end memory blocking\n";
		}
		else if(word == "P(run")
		{
			
			cout<< timeTaken <<" - process " << i << ": end processing action\n";
		}
		else if(word == "I(harddrive")
		{
			
			cout<< timeTaken<< " - process " << i << ": end hardrive input\n";
		}
		else if(word == "O(harddrive")
		{
			
			cout<< timeTaken<<" - process " << i << ": end hardrive output\n";
		}
		else if(word == "I(mouse")
		{
			
			cout<< timeTaken<< " - process " << i << ": end mouse input\n";
		}
		else if(word == "I(keyboard")
		{
			
			cout<< timeTaken <<" - process " << i << ": end keyboard input\n";	
		}
		else if(word == "O(monitor")
		{
			
			cout<< timeTaken<< " - process " << i << ": end monior output\n";
		}
		else if(word == "O(speaker")
		{
			
			cout<< timeTaken<< " - process " << i << ": end speaker output\n";
		}
		else if(word == "O(printer")
		{
			
			cout<< timeTaken<< " - process " << i << ": end printer output\n";
		}
	}
	if(logToFile == true)
	{
		fout.open(outputFile, std::ios_base::app);
	    if (!fout.is_open())//tries to open files
	    { 
	    	std::cerr << "Could not open output file\n";
	    	exit(EXIT_FAILURE);
	   	}	
		if(word == "S(start")
		{
			//do nothing
		}
		else if(word == "A(start") 
		{
			//do nothing
		}
		else if(word == "A(end")
		{
			fout << timeTaken << " - OS: Removing process " << i << endl;
		}
		else if(word == "M(allocate")
		{
			
			fout << timeTaken << " - process " << i << " memory allocated at 0x"<< std::hex << std::setw(8) << std::setfill('0') << address<< endl;
		}
		else if(word == "M(block")
		{
			fout << timeTaken << " - process " << i << ": end memory blocking\n";
		}
		else if(word == "P(run")
		{
			
			fout<< timeTaken <<" - process " << i << " :end processing action\n";
		}
		else if(word == "I(harddrive")
		{
			
			fout<< timeTaken<< " - process " << i << ": end hardrive input\n";
		}
		else if(word == "O(harddrive")
		{
			
			fout<< timeTaken<<" - process " << i << ": end hardrive output\n";
		}
		else if(word == "I(mouse")
		{
			
			fout<< timeTaken<< " - process " << i << ": end mouse input\n";
		}
		else if(word == "I(keyboard")
		{
			
			fout<< timeTaken <<" - process " << i << ": end keyboard input\n";	
		}
		else if(word == "O(monitor")
		{
			
			fout<< timeTaken<< " - process " << i << ": end monior output\n";
		}
		else if(word == "O(speaker")
		{
			
			fout<< timeTaken<< " - process " << i << ": end speaker output\n";
		}
		else if(word == "O(printer")
		{
			
			fout<< timeTaken<< " - process " << i << ": end printer output\n";
		}
	}
	
}


#endif


