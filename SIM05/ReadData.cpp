// Program Information /////////////////////////////////////////////////////////
/**
 * @file ReadData.cpp
 *
 * @brief Simulator implemntattion file for project 1 and 2 at
 *      Univserity of Nevada, Reno
 * 
 * @details This file includes the necessary function implementations for 
 *      assignment 1 and 2 of the CS 446/646 course simulator.
 *
 * @author Daniel Brown
 *
 * @version 1.01 (24 febuary 17)
 * @version 1.00 (3 February 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////
#ifndef READ_DATA_CPP
#define READ_DATA_CPP


// HEADER FILES ////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <vector>
#include "ReadData.h"
#include <stdlib.h>
#include <algorithm>

//used to make it simpler for outputing to monitor and new line
using std::endl;
using std::cout;
// Global variables//////////////////////
std::string outFile;


/** deletes all spaces
  /*@pre a string exists to pass to function
  /*@post all spaces are deleted
  /*param str string to be checked for extra spaces
  */
std::string ReadData:: delSpaces(std::string &str) 
{
   str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
   return str;
}

/** deletes unnecessary spaces
/*@pre a string exists to pass to function
/*@post all extra spaces are deleted
/*param str string to be checked for extra spaces
*/
std::string ReadData:: delUnnecessary(std::string &str)
{
    int size = str.length();
    for(int j = 0; j<=size; j++)
    {
        for(int i = 0; i <=j; i++)
        {
            if(str[i] == ' ' && str[i+1] == ' ')
            {
                str.erase(str.begin() + i);
            }
            else if(str[0]== ' ')
            {
                str.erase(str.begin());
            }
            else if(str[i] == '\0' && str[i-1]== ' ')
            {
                str.erase(str.end() - 1);
            }
        }
    }
    return str;
}

/** Searches vector for a string and returns -1 if not found
  /*@pre vector and string have been declared
  /*@post vector is searched and index returned 
  /*param string str string to search for
  /*  vector cycles to calculate times
  */
int searchVector(std::vector<cycleInfo> v, std::string str)
{
  int i = 0;
  while(i < v.size())
  {
    if(v[i].id == str)
      return i;
    else
      i++;
  }
  return -1;
}

/** Searches Data from config file for matching meta functions
  /*@pre config file has been read in
  /*@post config file is searched and if found index returned 
  /*param string str string to search for
  /*  vector cycles to calculate times
  */
int ReadData:: checkDescript(std::vector<cycleInfo> cycle, std::vector<cycleInfo>& data, std::string str)
{
  int runTime = 0;
  int index = -1;
   if(data.front().cycleTime >= 0)
    {
         index = searchVector(cycle, str);
         //checks if matching word is found from config
         if(index < 0)
          std::cerr << "No description from configuration file matching: " << str << ", skipping this process\n";

         runTime = cycle[index].cycleTime * data.front().cycleTime;
         data.erase(data.begin());
        }
        else
        {
          data.erase(data.begin());
          return -1;
        }
    return runTime;

}
  /** Gets Data from files and creates outputs based on the inputs
  /*@pre 
  /*@post all data is imported and errors handled 
  /*param int argc for command line operations
  /*  char*[] for command line operations
  */
std::vector<std::vector<cycleInfo>> ReadData:: getData(int argc, char *argv[])
{
  /**Variable Definitions**/
  std::queue<std::string> data;
  logToFile = false;
  logToMon = false;
  std::string word;
  std::string tempRead;
  std::string metaFileName;
  std::vector<cycleInfo> cycleType;
  std::string outFile;
  float versionNum = 0;
  std::vector<std::vector<cycleInfo>> returnVec;

  //checks for command word arguments by ensuring argc is greater than 2
	if(argc < 2)
	{
		std::cerr << " invalid number of command word files" << std::endl;
		exit(EXIT_FAILURE); 
	}//ends if

	std::ifstream fin;
	std::string checkConfExt = ".conf";

  std::queue<std::string> errorQueue;

	for(int i = 1; i < argc; i++)//opens and checks all command word arguments
	{
		//checks the file extension to ensure its a conf file using the string function find
		std::string commandwordName = argv[i];

		std:: size_t found = commandwordName.find(checkConfExt);

  		if(found == std::string::npos)
  		{	
  			std::cerr << "invalid file extension" << std::endl;
  			exit(EXIT_FAILURE); 
  		}

  		//if it is the correct file extension it will try to open the file using fstream
		fin.open(argv[i]);
    	if ( !fin.is_open())
    	{	
      		std::cerr<<"Could not open file" << std::endl;
      		exit(EXIT_FAILURE); //if failed to open the program will close      
      }//ends if

      //gets first line to check if the file is empty before reading it all in
      getline(fin,word);
      data.push(word);
      while(!fin.eof())
      {
        
        getline(fin,word);
        //places data into a queue and checks if it is an empty line
        if(word != "")
          data.push(word);
      }

      fin.close();

      //checks if file is empty by checking size of data queue
      if(data.size() <= 1 )
      {
        std::cerr << "Empty Configuration File in file named: " << commandwordName <<std::endl;
        exit(EXIT_FAILURE); 
      }
      
        //deletes extra spaces
        delUnnecessary(data.front());
        if(data.front() == "Start Simulator Configuration File")
        {
          data.pop();
        }
        else
        {
          std::cerr << "Misspelling in first line, read in from " << commandwordName << endl;
          data.pop();
          errorQueue.push(data.front());
        }


        //go through data in the queue
       while(!data.empty())
       {
        //gets rid of unwanted spaces
        delUnnecessary(data.front());

        //compares data from file with expected data using the string library overloaded operator
        if(data.front() == "End Simulator Configuration File" && data.front() == data.back() )
        {
         
          data.pop();
          break;
        }
        if(data.front() != "End Simulator Configuration File" && data.front() == data.back())
        {
          std::cerr <<"Misspelling in last line: '" << data.front() << "'\n";
          data.pop();
          errorQueue.push(data.front());
          break;
        }
          //checks for : to get cycle time using find
          found = data.front().find(":");       
          if(found == std::string::npos)
          {
              std::cerr << "Line missing ':' in line: '" << data.front() << "'" << endl;; 
              errorQueue.push(data.front());
              data.pop();
             
           }//ends if checking for :

           //ensures that there is a cycle time and not a blank entry
          if(data.front()[found] == '\0')
           {
            std::cerr << "Line missing expression after: " << data.front() << endl;
            data.pop();
           }//ends if 

          //if it is a valid entry and ':' is found it extracts first word and puts it into a vector using substring function
          else
          {
            tempRead = data.front().substr(0,found+1);
          }//ends else assigning the first word to a string

          if(tempRead == "Version/Phase:")
          {
            //sets word to the value after : using substring function
            word = data.front().substr(found+2, std::string::npos);
            versionNum = stof(word);
            data.pop();
            
          }//ends version/phase check

          else if(tempRead == "File Path:")
          {
            //sets word to the value after :
            word = data.front().substr(found+2, std::string::npos);
            found = word.find(".mdf");
            if(found == std::string::npos)
            { 
              std::cerr << "Invalid Meta Data file extension, read in from file '" << commandwordName << "' aborted\n";
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              metaFileName = word;
              data.pop();
            }
          }//ends file path check

          else if(tempRead == "Processor cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.id = "Processor";
            cycle.cycleTime = atoi((word).c_str());
            if(cycle.cycleTime <= 0)
            {
                std::cerr<< "Invalid cycle time on " << tempRead << endl;
                errorQueue.push(data.front());
                data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }

          }//ends Processor check

          else if(tempRead == "Monitor display time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Monitor";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }
          }//ends monitor check

          else if(tempRead == "Hard drive cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "hard drive";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }
          }//ends Hard drive check

           else if(tempRead == "Printer cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Printer";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }
          }//ends Printer check

          else if(tempRead == "Keyboard cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Keyboard";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();

            }
          }//ends keyboard check

          else if(tempRead == "Memory cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Memory";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }
          }//ends memory check

          else if(tempRead == "Mouse cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Mouse";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }
          }//ends mouse check

          else if(tempRead == "Speaker cycle time (msec):")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Speaker";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid cycle time on " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
            else
            {
              cycleType.push_back(cycle);
              data.pop();
            }
          }//ends speaker check

          else if(tempRead == "Log:")
          {
            //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            if(word == "Log to Both")
            {
              logToFile = true;
              logToMon = true;
              data.pop();
            }
            else if (word == "Log to File")
            {
              logToFile = true;
              logToMon = false;
              data.pop();
            }
            else if(word == "Log to Monitor")
            {
              logToMon = true;
              logToFile = false;
              data.pop();
            }
            else
            {
              std::cerr <<"Misspelling or missing item in line: '" << data.front() << "'\n";
              errorQueue.push(data.front());
              data.pop();
            }
          }//ends log check

           else if(tempRead == "Log File Path:")
          {
            //sets word to the value after : 
            outFile = data.front().substr(found+2, std::string::npos);
            outputFileName = outFile;
            data.pop();
            
          }//ends log file path check



          /***Start Assignment 3**/ 

          else if(tempRead == "System memory (kbytes):")
          {
             //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "System memory";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid system memory size " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
             else
            {
              cycleType.push_back(cycle);
              data.pop();

            }
          }

          else if(tempRead == "Memory block size (kbytes):")
          {
             //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Memory block size";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid memory block size " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
             else
            {
              cycleType.push_back(cycle);
              data.pop();

            }
          }

          else if(tempRead == "Printer quantity:")
          {
             //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Printer quantity";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid printer quantity " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
             else
            {
              cycleType.push_back(cycle);
              data.pop();

            }
          }

          else if(tempRead == "Hard drive quantity:")
          {
             //sets word to the value after : 
            word = data.front().substr(found+2, std::string::npos);
            cycle.cycleTime = atoi((word).c_str());
            cycle.id = "Harddrive quantity";
            if(cycle.cycleTime <= 0)
            {
              std::cerr << "Invalid hard drive quantity " << tempRead << endl;
              errorQueue.push(data.front());
              data.pop();
            }
             else
            {
              cycleType.push_back(cycle);
              data.pop();

            }
          }
          else if(tempRead == "CPU Scheduling Code:")
          {
             //sets word to the value after : */
            word = data.front().substr(found+2, std::string::npos);     
            cycle.cycleTime = 0;
            cycle.id = word;
            cycleType.push_back(cycle);
            data.pop();
          }


          else
          {
            std::cerr << "Misspelling in line '" << data.front() <<"'\n";
            errorQueue.push(data.front()); 
            data.pop();
        }//first word error check
      }//ends while data isnt empty
        //function calls to output Config file data and read in and output Meta Data 
        outputCfgData(cycleType,logToFile,logToMon, outFile);
        returnVec.push_back(getMetaData(metaFileName,cycleType)); 
        outputMeta(returnVec[0],logToFile, logToMon, outFile);
        cout << "\nEnd of file: " << commandwordName << endl << endl;
       
     }//ends for
     return returnVec;


     
}//ends getData

  /** Gets Data from files and creates outputs based on the inputs
  /*@pre 
  /*@post all data is imported and errors handled 
  /*param string meta name of the file
  /*  vector cycles to calculate times
  */
std::vector<cycleInfo> ReadData:: getMetaData(std::string meta, std::vector<cycleInfo>& cycles)
{

  /**Variable Definitions**/
  std::ifstream fin;
  std::vector<cycleInfo> data;
  std::string tempRead;
  std::vector<cycleInfo> metaCycles;
  std:: stack<cycleInfo> startStack;
  std:: stack<cycleInfo> endStack;
  std::string startCheck = "StartProgramMeta-DataCode:";
  std::string word;
  int index = 0;
  int i = 0;
  int startCounter = 0;
  cycleInfo cycle;
  std:: size_t found;
  int start = 0;
  int ready = 1;
  int running = 2;
  int waiting = 3;
  int exiting = 4;
  


 //open file check
  fin.open(meta);
  if ( !fin.is_open())
  { 
    std::cerr<<"Could not open file" << std::endl;
    exit (EXIT_FAILURE);
 
  }//ends if

  //Gets data from file and checks if it is empty
  getline(fin,tempRead);
  //removes spaces
  delSpaces(tempRead);

  if(tempRead != startCheck)
      {
        std::cerr <<"Misspelled or missing first/last line Program ending" << endl;
        exit(EXIT_FAILURE);
      }
      //reads in data until end of file
      while(!fin.eof())
      {
        //gets data from file until ')' is reached
        getline(fin,cycle.id,')');
        
        //gets cycle time for each meta data cycle
        fin>> cycle.cycleTime;
        //checks if there is no cycle time, if there isn't it ends the program
        if(fin.fail())
        {
          //searches for End of meta file and if it isnt there it ends the program
          found = cycle.id.find('E');
          if(found != std::string::npos)
          {
              break;
          }
          //outputs which number descriptor has an issue if any
          std::cerr << "Descriptor number " << i+1 << " has an error, program aborted" << endl;
          exit(EXIT_FAILURE);
        }
        //puts data into a vector
        data.push_back(cycle);
        i++;
            
      }
      fin.close();
      //if it is an empty file it will end the program
      if(data.size() <= 1 )
      {
        std::cerr << "Empty Meta File in file named: " << meta <<std::endl;
        exit(EXIT_FAILURE);
      }


  while(!data.empty())
  {
      //deletes extra spaces to make data easier to go through
      delSpaces(data.front().id);

      //searches for '(' for each descriptor
      found = data.front().id.find('(');

      if(found == std::string::npos)
      { 
        std::cerr << "Missing '('" << std::endl;
        exit(EXIT_FAILURE);
      }
      //sets the string from one char before '(' to get entire descriptor
      word = data.front().id.substr(found-1, std::string::npos);
      
      /**Organizes data and puts it into a vector using string == operator**/


      if(word == "S(start") 
      {
       if(data.front().cycleTime == 0)
       {
          startStack.push(data.front());
          cycle.id = word;
          cycle.cycleTime = 0;
          metaCycles.push_back(cycle);
          data.erase(data.begin());
       }
       else
       {
         std::cerr << "Invalid cycle time for: " << word << endl;
         data.erase(data.begin());
       }
       
      }

     else if(word == "S(end")
     {
       if(data.front().cycleTime == 0)
       {
        if(!startStack.empty())
        { 
            startStack.pop();
        }
        else
        {
          std::cerr << "Warning: Meta file contains " << word << ") without a matching S(start)\n";
        }

         data.erase(data.begin());
       }
       else
       {
         std::cerr << "Invalid cycle time for: " << word << ")" << endl;
         data.erase(data.begin());
       }
       
     }
      else if(word == "A(start") 
      {
        if(data.front().cycleTime == 0)
        {
          endStack.push(data.front());
          startCounter++;
          //first application running so it is in ready state
          if(startCounter == 1)
            cycle.pcb.processState = ready;
          
          cycle.cycleTime = 0;
          cycle.id = word;
          metaCycles.push_back(cycle);
          data.erase(data.begin());
          

        }
        else
        {
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
          data.erase(data.begin());
        }
        
      }   
      else if(word == "A(end")
      {
        if(data.front().cycleTime == 0)
        {
          cycle.cycleTime = 0;
          cycle.id = word;
          metaCycles.push_back(cycle);
          if(!endStack.empty())
          {
              endStack.pop();
          }
          else
          {
            std::cerr << "Warning: Meta file contains " << word << ") without a matching A(start)\n";
          }
          data.erase(data.begin());
        }
        else
        {
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
          data.erase(data.begin());
        }
        
      } 
      else if(word == "M(allocate" || word == "M(block")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Memory");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
      }

      else if(word == "P(run")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Processor");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
          
      }
      else if(word == "I(harddrive" || word == "O(harddrive")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"hard drive");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
          
      }
      else if(word == "I(mouse")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Mouse");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
      }
      else if(word == "I(keyboard")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Keyboard");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
      }
      else if(word == "O(monitor")// word == "O(speaker" )
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Monitor");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
      }
      else if(word == "O(speaker")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Speaker");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
      }
      else if(word == "O(printer")
      {
        cycle.pcb.processState = ready;
        cycle.cycleTime = checkDescript(cycles,data,"Printer");
        if(cycle.cycleTime <= 0)
          std::cerr << "Error in cycle time for: " << word << ")" << endl;
        else
        {
          cycle.id = word;
          metaCycles.push_back(cycle);
        }
      }

      else
        {
          std::cerr << "Mispelling or invalid descriptor in Meta data file: '" << word << ")', skipping process\n";
          data.erase(data.begin());
        }
        
  }
  /**check for end descriptors using a stack **/

  if(!startStack.empty())
  {
    std::cerr << "Warning: Meta file contains S(start) without an S(end)\n";
  }
  if(!endStack.empty())
  {
    std::cerr << "Warning: Meta file contains A(start) without an A(end)\n";
  }
  //put the new data for assignment 3 into vector to get information in the simulator
  for(int i = 0; i < cycles.size(); i++)
  {
     if(cycles[i].id == "Harddrive quantity")
     {
     metaCycles.push_back(cycles[i]);
     }
     else if(cycles[i].id == "Printer quantity")
     {
       metaCycles.push_back(cycles[i]);
     }
    else if(cycles[i].id == "Memory block size")
     {
       metaCycles.push_back(cycles[i]);

     }
     else if(cycles[i].id == "System memory")
     {
       metaCycles.push_back(cycles[i]);
     }
     else if(cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
     {
      metaCycles.push_back(cycles[i]);
     }
  }

  return metaCycles;
      

}//ends getMetaData

/** outputs data based on configuration file
  /*@pre configuration file has been handled
  /*@post all data is outputed based on config file 
  /*param bool logToFile, logToMon logs to file or monitor based on config file
  /*  vector to output data
  /*  string outfile name of the file to log to if needed
  */
bool ReadData:: outputCfgData(const std::vector<cycleInfo>& cycles, bool logToFile, bool logToMon, std::string outFile) const
{

  std::ofstream fout;
  //if it is to be logged to both monitor and file
  if(logToMon == true && logToFile == true)
  {
    fout.open(outFile);
    if (!fout.is_open())//tries to open files
    { 
      std::cerr<<"Could not open output file, printing only to monitor" << std::endl;
      cout << "Configuration File Data" << endl;
      for(int i = 0; i < cycles.size(); i++)
      {
         if(cycles[i].id == "Harddrive quantity")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
         }
         else if(cycles[i].id == "Printer quantity")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
         }
        else if(cycles[i].id == "Memory block size")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
         }
         else if(cycles[i].id == "System memory")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
         }
         else if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
         {
          cout << "CPU scheduling code = " << cycles[i].id << endl;
         }
        else
        {
          cout << cycles[i].id << " = " << cycles[i].cycleTime
          << " ms/cycle\n";
        }
      }
      cout << "logged to monitor only because of issue opening file\n\n";
      return false;
    }//ends if
    else
       { 
        cout << "Configuration File Data" << endl;
        fout << "Configuration File Data" << endl;
         for(int i = 0; i < cycles.size(); i++)
          {
            if(cycles[i].id == "Hard drive quantity")
             {
              cout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
              fout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
             }
             else if(cycles[i].id == "Printer quantity")
             {
              cout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
              fout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
             }
            else if(cycles[i].id == "Memory block size")
             {
              cout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
              fout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
             }
             else if(cycles[i].id == "System memory")
             {
              cout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
              fout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
             }
             else if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
            {
              fout << "CPU scheduling code = " << cycles[i].id << endl;
              cout << "CPU scheduling code = " << cycles[i].id << endl;
            }
          else
          {
           cout << cycles[i].id << " = " << cycles[i].cycleTime
           << " ms/cycle\n";
           fout << cycles[i].id << " = " << cycles[i].cycleTime
           << " ms/cycle\n";
          }
          } 
          cout<< "Logged to: Monitor and " << outFile << endl << endl;
          fout<< "Logged to: Monitor and " << outFile << endl << endl;
          fout.close();
          return true;
        }
    }//ends if supposed to be logged to mon and file

    //log to moitor only
    else if(logToMon == true && logToFile == false)
    {
      cout << "Configuration File Data" << endl;
      for(int i = 0; i < cycles.size(); i++)
      {
        if(cycles[i].id == "Harddrive quantity")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
         }
         else if(cycles[i].id == "Printer quantity")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
         }
        else if(cycles[i].id == "Memory block size")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
         }
         else if(cycles[i].id == "System memory")
         {
          cout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
         }
         else if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
         {
          cout << "CPU scheduling code = " << cycles[i].id << endl;
         }
         else
          { cout << cycles[i].id << " = " << cycles[i].cycleTime
            << " ms/cycle\n";
          }
      } 
      cout << "Logged to: Monitor only\n" << endl;
      return true;
    }//ends log to monitor only
    //log to file only
    else if(logToMon == false && logToFile == true)
    {
      fout.open(outFile);
      if (!fout.is_open())//tries to open files
      { 
        std::cerr<<"Could not open output file, aborting print" << std::endl;
        return false;
      }
      else
      {
        fout << "Configuration File Data" << endl;
        for(int i = 0; i < cycles.size(); i++)
        {
          if(cycles[i].id == "Harddrive quantity")
         {
          fout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
         }
         else if(cycles[i].id == "Printer quantity")
         {
          fout << cycles[i].id << " = " << cycles[i].cycleTime << endl;
         }
        else if(cycles[i].id == "Memory block size")
         {
          fout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
         }
         else if(cycles[i].id == "System memory")
         {
          fout << cycles[i].id << " = " << cycles[i].cycleTime << " kilobytes" << endl;
         }
         else if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
         {
          fout << "CPU scheduling code = " << cycles[i].id << endl;
         }
         else
          {
            fout << cycles[i].id << " = " << cycles[i].cycleTime
            << " ms/cycle\n";
          }
        } 
        fout<< "Logged to: " << outFile << endl << endl;
        fout.close();
        return true;
      }
    }//ends log to file only

    else 
    {
      std::cerr<< "Invalid path to log Configuration data" << endl;
      return false;
    }
}//end outoutCfgdata

  /** outputs data based on configuration and meta file
  /*@pre configuration and meta files have been handled
  /*@post all data is outputed based on files 
  /*param bool logToFile, logToMon logs to file or monitor based on config file
  /*  vector to output data
  /*  string outfile name of the file to log to if needed
  */
bool ReadData:: outputMeta(const std::vector<cycleInfo>& cycles, bool logToFile, bool logToMon, std::string outFile) const
{
  std::ofstream fout;

  if(logToMon == true && logToFile == true)
  {
    fout.open(outFile, std::ios_base::app);
    if (!fout.is_open())//tries to open files
    { 
      std::cerr<<"Could not open output file, printing only to monitor" << std::endl;
      cout << "\nMeta Data Metrics" << endl;
      for(int i = 0; i < cycles.size()-4; i++)
      {
        if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
        {
          //do nothing
        }
        else
          cout << cycles[i].id << ") - " << cycles[i].cycleTime
        << " ms\n";
      }
      return false;
    }//ends if

    else
       { 
        cout << "\nMeta Data Metrics" << endl;
        fout << "\nMeta Data Metrics" << endl;
         for(int i = 0; i < cycles.size()-4; i++)
          {
            if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
            {
            //do nothing
            }
          else
           cout << cycles[i].id << ") = " << cycles[i].cycleTime
           << " ms\n";
           fout << cycles[i].id << ") = " << cycles[i].cycleTime
           << " ms\n";
          } 

          fout.close();
          return true;
        }
    }//ends if supposed to be logged to mon and file

    else if(logToMon == true && logToFile == false)
    {
      cout << "\nMeta Data Metrics" << endl;
      for(int i = 0; i < cycles.size()-4; i++)
      {
        if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
        {
            //do nothing
        }
        else
          cout << cycles[i].id << ") = " << cycles[i].cycleTime
          << " ms\n";
      } 
      return true;
    }

    else if(logToMon == false && logToFile == true)
    {
      fout.open(outFile, std::ios_base::app );
      if (!fout.is_open())//tries to open files
      { 
        std::cerr<<"Could not open output file, aborting print" << std::endl;
        return false;
      }
      else
      {
        fout << "\nMeta Data Metrics" << endl;
        for(int i = 0; i < cycles.size()-4; i++)
        {
          if( cycles[i].id == "FIFO" || cycles[i].id == "SJF" ||  cycles[i].id == "PS")
            {
            //do nothing
            }
          else
            fout << cycles[i].id << ") = " << cycles[i].cycleTime
            << " ms\n";
        } 
        fout.close();
        return true;
      }
    }
    else 
    {
      std::cerr<< "Invalid path to log Configuration data" << endl;
      return false;
    }
}//ends output meta data
/**returns if data should be printed to monitor
  /*@pre config file has been read in
  /*@post bool value to print to mon is returnedr
  */
bool ReadData:: getLogMon()
{
  return logToMon;
}
/**returns if data should be printed to file
  /*@pre config file has been read in
  /*@post bool value to print to file is returned
  */
bool ReadData:: getLogFile()
{
  return logToFile;
}
std::string& ReadData:: getFileName()
{
  return outputFileName;
}

#endif