/*	Daniel McNulty II
*
*	Option.h
*/

#ifndef Option_H
#define Option_H

#include <iostream>
using namespace std;

class Option
{
protected:
	virtual void Copy(const Option& source);	// Copy the values of Option datamembers from one Option object to another

public:
	enum OptionType optionType;		// Option type, call or put

public:
	// Constructors
	Option();									// Default constructor
	Option(const Option& source);				// Copy constructor
	Option(const enum OptionType& optionType);	// Constructor that accepts option type
	// Destructor
	virtual ~Option();							// Default destructor

	// Functionality
	// Functions that calculate option price and sensitivities
	virtual double Price() const = 0;
	virtual double Delta() const;
	virtual double Gamma() const;

	// Assignment operator
	Option& operator = (const Option& source);

	// Modifier functions
	void toggle();		// Change option type
};

// Implementation of enum OptionType
enum OptionType
{
	Put,
	Call
};

#endif