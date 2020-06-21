/*	Daniel McNulty II
*
*	OptionExceptions.h
*/

#ifndef OptionExceptions_H
#define OptionExceptions_H

#include <iostream>
using namespace std;

// OptionException base class
class OptionException
{
public:
	// Constructors
	OptionException() {};	// Default constructor
							// Destructors
	virtual ~OptionException() {};	// Default destructor

									// Functionality
	virtual string GetMessage() = 0;	// Get error message for the exception
};

// NotImplementedException class derived from the OptionException base class
class NotImplementedException : public OptionException
{
private:
	// Private data member that holds the function name of a function that isn't implemented
	string FunctionName;

public:
	// Constructors
	NotImplementedException() : OptionException(), FunctionName("An unspecified function") {};	// Default Constructor
	NotImplementedException(string fcn) : OptionException(), FunctionName(fcn) {};				// Constructor accepting not implemented function name
																								// Destructors
	virtual ~NotImplementedException() {};														// Default destructor

																								// Functionality
	virtual string GetMessage()		// Generate and return a string with an error message for the not implemented function
	{
		string Message = "ERROR : " + FunctionName + " has been called, but no implementation of it exists in the derived class it has been called for.";
		return Message;
	}
};

#endif