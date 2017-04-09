// Program Information /////////////////////////////////////////////////////////
/**
 * @file ReadData.h
 *
 * @brief Simulator implemntattion file for project 1 and 2 at
 *		  Univserity of Nevada, Reno
 * 
 * @details This file includes the necessary function implementations for 
 *			assignment 1 and 2 of the CS 446/646 course simulator.
 *
 * @author Daniel Brown
 *
 * @version 1.01 (24 febuary 17)
 * @version 1.00 (3 February 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////
#ifndef READ_DATA_H
#define READ_DATA_H


// HEADER FILES ////////////////////////////////////////////////////////////////
#include <queue>
#include <vector>
#include <string>
struct PCB
{
	int processState;
	int memBlock;
	int systemMem;

};
struct cycleInfo
{
	std::string id;
 	int cycleTime;
 	PCB pcb;

}; 
 class ReadData
 {
private:
	cycleInfo cycle;
	bool logToFile;
	bool logToMon;
	std::string outputFileName;

public:
	/** Gets Data from files and creates outputs based on the inputs
	/*@pre 
	/*@post all data is imported and errors handled 
	/*param int argc for command line operations
	/*	char*[] for command line operations
	*/
	std::vector<std::vector<cycleInfo> >  getData(int argc, char *argv[]);

	/** Gets Data from files and creates outputs based on the inputs
	/*@pre 
	/*@post all data is imported and errors handled 
	/*param string meta name of the file
	/*	vector cycles to calculate times
	*/
	std::vector<cycleInfo> getMetaData(std::string meta, std::vector<cycleInfo>& cycles);

	/** outputs data based on configuration file
	/*@pre configuration file has been handled
	/*@post all data is outputed based on config file 
	/*param bool logToFile, logToMon logs to file or monitor based on config file
	/*	vector to output data
	/*	string outfile name of the file to log to if needed
	*/
	bool outputCfgData(const std::vector<cycleInfo>& cycles,bool logToFile, bool logToMon, std::string outFile) const;

	/** outputs data based on configuration and meta file
	/*@pre configuration and meta files have been handled
	/*@post all data is outputed based on files 
	/*param bool logToFile, logToMon logs to file or monitor based on config file
	/*	vector to output data
	/*	string outfile name of the file to log to if needed
	*/
	bool outputMeta(const std::vector<cycleInfo>& cycles, bool logToFile, bool logToMon, std::string outFile) const;

	/** deletes all spaces
	/*@pre a string exists to pass to function
	/*@post all spaces are deleted
	/*param str string to be checked for extra spaces
	*/
	std::string delSpaces(std::string &str);

	/** deletes unnecessary spaces
	/*@pre a string exists to pass to function
	/*@post all extra spaces are deleted
	/*param str string to be checked for extra spaces
	*/
	std::string delUnnecessary(std::string &str);

	/** Searches Data from config file for matching meta functions
	/*@pre config file has been read in
	/*@post config file is searched and if found index returned 
	/*param string str string to search for
	/*	vector cycles to calculate times
	*/
	int checkDescript(std::vector<cycleInfo> cycle, std::vector<cycleInfo>& data, std::string str);

	
	/**returns if data should be printed to monitor
	/*@pre config file has been read in
	/*@post bool value to print to mon is returnedr
	*/
	bool getLogMon();

	/**returns if data should be printed to file
	/*@pre config file has been read in
	/*@post bool value to print to file is returned
	*/
	bool getLogFile();

	/**returns output file name
	/*@pre config file has been read in
	/*@post output file name is returned
	*/
	std::string& getFileName();

 };

#endif