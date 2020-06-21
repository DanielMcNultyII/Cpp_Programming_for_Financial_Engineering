/*	Daniel McNulty II
*
*	Option.cpp
*/

#include "Option.h"
#include "OptionExceptions.h"
#include <iostream>
using namespace std;

// PROTECTED MEMBER FUNCTIONS
void Option::Copy(const Option& source)		// Copy the values of Option datamembers from one Option object to another
{
	optionType = source.optionType;
}

// PUBLIC MEMBER FUNCTIONS
// Constructors
Option::Option() : optionType(Call) { /*Init();*/ }													// Default constructor

Option::Option(const Option& source) : optionType(source.optionType) { /*Copy(source);*/ }			// Copy constructor

Option::Option(const enum OptionType& OptType) : optionType(OptType) {}								// Constructor that accepts option type

// Destructor
Option::~Option() {}	// Default destructor

// Functionality
double Option::Delta() const
{
	throw NotImplementedException("Delta()");		// Throw NotImplementedException if Delta() is not declared
}

double Option::Gamma() const
{
	throw NotImplementedException("Gamma()");		// Throw NotImplementedException if Gamma() is not delared
}

// Assignment Operator
Option& Option::operator = (const Option& source)
{
	if (this == &source)	// Avoid assigning to itself
	{
		return *this;
	}
	else
	{
		Copy(source);
		return *this;
	}
}

// Toggle function to change option type
void Option::toggle()
{
	optionType = ((optionType == Call) ? Put : Call);
}