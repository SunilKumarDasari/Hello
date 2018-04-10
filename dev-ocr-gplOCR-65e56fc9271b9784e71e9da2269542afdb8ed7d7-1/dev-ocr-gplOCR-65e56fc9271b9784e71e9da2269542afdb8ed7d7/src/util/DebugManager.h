#ifndef DebugManager_H
#define DebugManager_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iostream>
/*namespace std {
//#include <cstdlib>
};
#include <math.h>
*/

//#define windows
#define textMode

class DebugManager{
	static std::ofstream* currStream;
	static void FlushDebugFileOStream();

public:
	static std::ofstream& GetDebugFileOStream(std::string className);
	static void CloseDebugFileOStream();
	static char directoryPath[1024];
	static char debugPath[1024];
	static bool isPathDefined;
	static void setDebugPath(char* inputDebugPath);
	/*
		Returns name of the debug file.
	*/
	static const char* getLogFileName();
	static const char* getDebugDirectoryPath();
	/*		
		This function decides if debug images pertaining to the passed function need to be created or not. 
		This is used in case the output is into the console using a printf statement.
		\param[in] className This class name is compared to see if it's debug image should be created or not
		\param[out] bool true if the debug image should be skipped (not created), false in case the debug image needs to be created.
	
	*/
	static bool noDebugImage(std::string className);
	/*
		This function decides if images should be displayed or not. For example, before doing imshow, one should make a call to this function.

		\param[in] className This class name is compared to see if the image should be displayed or not.
		\param[out] bool true if the image should be skipped (not displayed), false in case the image needs to be displayed.

	*/
	static bool noImageDisplay(std::string className);

	/*		
		This function decides if debug information pertaining to the passed function need to be output onto the screen.
		This is used in case the output is into the console using a printf statement.
		\param[in] className This class name is compared to see if it's debug should be printed or not
		\param[out] bool true if the debug information should be skipped (not printed), false in case the debug information needs to be printed.
	*/
	static bool noDebug(std::string className);

	/*		
		This function decides if debug information pertaining to the passed function need to be output.
		This is used in case the output is into the console using a printf statement.
		\param[in] className This class name is compared to see if it's debug should be printed or not
		\param[out] bool true if the debug information should be skipped (not printed), false in case the debug information needs to be printed.
	*/
	static bool noPrintf(std::string className){
	  if(className.compare("Recognizer::gsc_nearest_neighbor_batch") == 0)
			return false;
		if (className.compare("Recognizer::grad_neural_net_batch") == 0)
			return false;
		return true;
	};

	/**
		This uses vsnprintf to create a char* message of the exact length that is needed to represent the entered 
		variable arguments.
		\param[in] fmt The format and other arguments similar to the variadic printf function
		\return a char* containing the required message, created using malloc and realloc, and should hence be freed using free. 
	*/
	static char *make_message(const char* fmt, ...); 

	/*
		Making use of variadic functions in C to control printf statements
		http://www.eskimo.com/~scs/C-faq/q15.5.html

		\param[in] className Name of the class that invoked this function. This value is used to determine if debug information for that class is to be printed or not.
		\param[in] printfStart First variable of the printf statement.
		\param[in] Since printf can take multiple arguments, these are passed as is the standard procedure in variadic functions
	*/
	static void printfDebugMessage(std::string className, char* printfStart, ...){
		if (noPrintf(className))
			return;
		va_list argp;
		va_start(argp, printfStart);
		//printf("\n%s:\n", className);
		vprintf(printfStart, argp);
		va_end(argp);
	};

	/*
		Making use of variadic functions in C to control printf statements
		http://www.eskimo.com/~scs/C-faq/q15.5.html

		\param[in] className Name of the class that invoked this function. This value is used to determine if debug information for that class is to be printed or not.
		\param[in] file_stream The file stream to which you need to have the printing done. 
		\param[in] printfStart First variable of the printf statement.
		\param[in] Since printf can take multiple arguments, these are passed as is the standard procedure in variadic functions
	*/
	static void fprintfDebugMessage(std::string className, FILE* file_stream, char* printfStart, ...){
		if (noPrintf(className))
			return;
		va_list argp;
		va_start(argp, printfStart);
		//printf("\n%s:\n", className);
		vfprintf(file_stream, printfStart, argp);
		va_end(argp);
	};

	/*
		Making use of variadic functions in C to write printf statement contents to the log file
		http://www.eskimo.com/~scs/C-faq/q15.5.html

		\param[in] className Name of the class that invoked this function. This value is used to determine if debug information for that class is to be printed or not.
		\param[in] printfStart First variable of the printf statement.
		\param[in] Since printf can take multiple arguments, these are passed as is the standard procedure in variadic functions
	*/
	static void writePrintfToLog(std::string className, const char *printfStart, ...){
		if (noDebug(className))
			return;
		//try{
				int n, size = 100;
				char *p;
				va_list ap;
				if ((p = (char *)malloc (size * sizeof(char))) == NULL){
				  const char *call = "DebugManager::writePrintfToLog";
					DebugManager::printfToLog(true, call, "Mem error 1\n");
					return;
				}
				while (1) {
					// Try to print in the allocated space. 
					va_start(ap, printfStart);
					n = vsnprintf (p, size, printfStart, ap);
					va_end(ap);
					// If that worked, return the string.
					if (n > -1 && n < size){
						DebugManager::printfToLog(true, "DebugManager::writePrintfToLog", "Mem filled correctly %d, %d\n", n, size);
					   break;
					}
					// Else try again with more space. 
					if (n > -1)    // glibc 2.1 
					   size = n+1; // precisely what is needed
					else           // glibc 2.0
					   size *= 2;  // twice the old size
					if ((p = (char *)realloc (p, size * sizeof(char))) == NULL){
						DebugManager::printfToLog(true, "DebugManager::writePrintfToLog", "Mem error 2 %d, %d\n", n, size);
					   return;
					}
				}

			writeToLog(className, p, false);
			free(p);
		/*}catch(exception e){
			printf("exception %s", e.what());
			writeToLog(className, printfStart, "The error statement output was longer than 300 characters, ignored");
			return;
		}*/
	};

	/*
		Making use of variadic functions in C to write printf statement contents to the log file
		http://www.eskimo.com/~scs/C-faq/q15.5.html

		\param[in] exact Write data into the file exactly as a fprintf command would write when called with a pointer to the 
				   log file and printfStart and te variadic function.
		\param[in] className Name of the class that invoked this function. This value is used to determine if debug information for that class is to be printed or not.
		\param[in] printfStart First variable of the printf statement.
		\param[in] Since printf can take multiple arguments, these are passed as is the standard procedure in variadic functions
	*/
	static void printfToLog(bool exact, const char* className, const char* printfStart, ...){
		if (noDebug(className))
			return;
		//try{
/*			char printf_output[15000] = "";
			va_list argp;
			va_start(argp, printfStart);
			vsprintf(printf_output, printfStart, argp);
			va_end(argp);		
			writeToLog(className, printf_output, exact);
*/
				int n, size = 100;
				char *p;
				va_list ap;
				if ((p = (char *)malloc (size * sizeof(char))) == NULL){
					DebugManager::printfToLog(true, "DebugManager::printfToLog", "Mem error 1\n");
					return;
				}
				while (1) {
					// Try to print in the allocated space. 
					va_start(ap, printfStart);
					n = vsnprintf (p, size, printfStart, ap);
					va_end(ap);
					// If that worked, return the string.
					if (n > -1 && n < size){
						DebugManager::printfToLog(true, "DebugManager::printfToLog", "Mem filled correctly %d, %d\n", n, size);
					   break;
					}
					// Else try again with more space. 
					if (n > -1)    // glibc 2.1 
					   size = n+1; // precisely what is needed
					else           // glibc 2.0
					   size *= 2;  // twice the old size
					if ((p = (char *)realloc (p, size * sizeof(char))) == NULL){
						DebugManager::printfToLog(true, "DebugManager::printfToLog", "Mem error 2 %d, %d\n", n, size);
					   return;
					}
				}
			writeToLog(className, p, exact);	
			free(p);
		/*}catch(exception e){
			printf("exception %s", e.what());
			writeToLog(className, printfStart, "The error statement output was longer than 300 characters, ignored");
			return;
		}*/
	};

	/*
		Static function used to output either a warning or an error statement
	
		\param[in] printfStart First variable of the printf statement.
		\param[in] Since printf can take multiple arguments, these are passed as is the standard procedure in variadic functions
	*/
	static void warning(std::string className, char* printfStart, ...){
		if (noDebug(className))
			return;

		va_list argp;
		va_start(argp, printfStart);
		vfprintf(stderr, printfStart, argp);
		va_end(argp);
	};
	
	/*
		Static function used to output either a warning or an error statement
	
		\param[in] printfStart First variable of the printf statement.
		\param[in] Since printf can take multiple arguments, these are passed as is the standard procedure in variadic functions
	*/
	static void fatal_error(std::string className, char* printfStart, ...){
		if (noDebug(className))
			return;

		va_list argp;
		va_start(argp, printfStart);
		vfprintf(stderr, printfStart, argp);
		va_end(argp);
		exit(-1);
	};


	// Throughout the library's codebase this function is called
	// to get the stream that debug output should be sent to.
	static std::ostream& GetCoutStream(std::string className);

	/*
		Write message to a log file.

		\param[in] className Name of the class that invoked this function. This value is used to determine if debug information for that class is to be printed or not.
		\param[in] specialMessage Sometimes, a special message needs to be used to relate 2 or more consecutive debug statements easily. 
		\param[in] message This message is appended to "specialMessage" and written to the log file.
	*/
	static void writeToLog(std::string className, char* specialMessage, char* message){
		if (noDebug(className))
			return;
		#ifdef textMode
			//std::string errMsg = (*new std::string(className))+":"/*+getTime()*/+message;
			std::string *errMsg = new std::string(className);
			errMsg->append(":"); 
			errMsg->append(message); 
		#endif
		#ifndef textMode
			//std::string *errMsg = (*new std::string(className))+":"/*+getTime()*/+message+"<br>\n";
			std::string *errMsg = new std::string(className);
			errMsg->append(":"); 
			errMsg->append(message); 
			errMsg->append("<br>\n"); 
		#endif
		writeToFile(getLogFileName(), errMsg);
		delete (errMsg);

	};

	/*
		Write message to the log file.

		\param[in] className Name of the class that invoked this function. This value is used to determine if debug information for that class is to be printed or not.
		\param[in] message This message is appended to "specialMessage" and written to the log file.
	*/
	static void writeToLog(std::string className, char* message, bool exact){
		if (noDebug(className))
			return;
		std::string *errMsg = new std::string("");
		
		if (exact){
			errMsg->append(message);
		}else{
			errMsg->append(className)/*+getTime()*/;
			errMsg->append(":");
			errMsg->append(message);
			#ifdef textMode
				errMsg->append("\n");
			#endif
			#ifndef textMode
				errMsg->append("<br>\n");
			#endif

		}
		writeToFile(getLogFileName(), errMsg);
		delete (errMsg);
	};


	/*
		gets the local time in ascii format. Example: Thu Aug 05 17:26:57 2004
	*/
	static char* getTime(){
		time_t rawtime;
		tm* ptm;
		time(&rawtime);
		ptm = localtime ( &rawtime );
		char* time = asctime(ptm);
		time[24] = ':';
		return time;
	};

	/*
		This function writes input data to a temporary html "temp_output.html" file in the specified directory. 
		If another file of the same name is present at that location, that file is overwritten.
		\param[in] directoryPath - The directory into which you have to write the HTML file.
		\param[in] message - The string item which contains the HTML code you need to write. 
	*/
	static void writeSampleHTMLOutput(std::string* directoryName, std::string* message){
		writeSampleHTMLOutput(directoryName, "\\temp_output.html", message);
	};
	
	/*
		This function writes input data to the destination file in the specified directory. 
		If another file of the same name is present at that location, that file is overwritten.

		\param[in] directoryPath - The directory into which you have to write the HTML file.
		\param[in] fileName - The destination HTML file. 
		\param[in] message - The string item which contains the HTML code you need to write. 
	*/
	static void writeSampleHTMLOutput(std::string* directoryName, const char* fileName, std::string* message){
		std::string* tempHtmlFilePath = new std::string(directoryName->c_str());
		tempHtmlFilePath->append(fileName);
		overwriteFile(tempHtmlFilePath->c_str(), message);
		delete(tempHtmlFilePath);
	};

	/*
		This clears the log file obtained from getLogFileName() and initializes a new log with 
		current date and time in it.
	*/
	static void resetLogFile(){
		FILE* stream = fopen(DebugManager::getLogFileName(), "w");
		if (! stream)
			return;
		
		fprintf(stream, " ");
		
		
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);		
		
		#ifdef textMode
			fprintf(stream, "Log Started %s \n", asctime(timeinfo));
		#endif
		#ifndef textMode
			fprintf(stream, "<HTML><BODY>");
			fprintf(stream, "Log Started %s <br>", asctime(timeinfo));
		#endif
		
		fflush(stream);
		fclose(stream);
			
	};

private:
	/*
		A wrapper to the printf statement so that I need not bother with flushing the stream on each write.
		This version appends to the file, if a file is already present.

		\param[in] logFileName File to which I need to write data
		\param[in] message message that needs to be printed
	*/
	static void writeToFile(const char* logFileName, std::string* message){
		//printf("%s", message->c_str());
		
		FILE* stream = fopen(logFileName, "a+");

		if (stream){
			fprintf(stream, "%s", message->c_str());
			fflush(stream);
			fclose(stream);
		}
	};

	/*
		A wrapper to the printf statement so that I need not bother with flushing the stream on each write.
		This version overwrites the file, if present.

		\param[in] logFileName File to which I need to write data
		\param[in] message message that needs to be printed		
	*/
	static void overwriteFile(const char* logFileName, std::string* message){
		FILE* stream = fopen(logFileName, "w");
		if (stream){
			fprintf(stream, "%s", message->c_str());
			fflush(stream);
			fclose(stream);
		}
	};

};

#endif
