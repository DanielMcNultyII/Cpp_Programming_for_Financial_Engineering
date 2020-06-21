/*	Daniel McNulty II
*
*	PerpetualAmericanOption.h
*/

#ifndef PerpetualAmericanOption_H
#define PerpetualAmericanOption_H

#include "Option.h"
#include <string>
#include <vector>
using namespace std;

class PerpetualAmericanOption : public Option
{
private:
	void Copy(const PerpetualAmericanOption& source);		// Copy all values from another PerpetualAmericanOption object

public:
	// Data members for the variables involved in perpetual American option pricing
	double K;		// Strike price
	double sig;		// Volatility
	double r;		// Risk free interest rate
	double U;		// Current price of the underlying asset
	double b;		// Cost of carry

public:
	// Constructors
	PerpetualAmericanOption();																								// Default constructor
	PerpetualAmericanOption(const PerpetualAmericanOption& Opt);															// Copy constructor
	PerpetualAmericanOption(const enum OptionType type);																	// Constructor that accepts option type
	PerpetualAmericanOption(double newK, double newSig, double newR, double newU, double newB);								// Constructor that accepts all derived class data
	PerpetualAmericanOption(double newK, double newSig, double newR, double newU, double newB, const enum OptionType type);	// Constructor that accepts all base and derived class data
	// Destructors
	virtual ~PerpetualAmericanOption();			// Default destructor

	// Functionality
	double Price() const;						// Calculate the price of the given option
	double PriceWithS(double newU) const;		// Calculate price using the set K,sig, r, and b with a different underlying value

	// Assignment operator
	PerpetualAmericanOption& operator = (const PerpetualAmericanOption& Opt);
};

enum PerpAmerOptParam			// enum to chose the varying parameter in the GenerateMatrix function
{
	Strike,
	Sigma,
	Interest,
	Underlying,
	Cost_Of_Carry
};


// Global Perpetual American Option Functions
// Vector and Matrix Generators
vector<double> GenerateMeshArray(double begin, double end, int n);					// Mesh generator

vector<vector<double>> GenerateParameterMatrix(double K, double Start_sig, double r, double U, double b, double End_sig, int steps, PerpAmerOptParam VariedParameter);					// General Parameter Varying Matrix Generator
vector<vector<double>> GenerateStrikeMatrix(double StartK, double sig, double r, double U, double b, double EndK, int steps);				// Strike Price Varying Matrix Generator
vector<vector<double>> GenerateVolatilityMatrix(double K, double Start_sig, double r, double U, double b, double End_sig, int steps);		// Volitility Varying Matrix Generator
vector<vector<double>> GenerateInterestMatrix(double K, double sig, double Start_r, double U, double b, double End_r, int steps);			// Interest Rate Varying Matrix Generator
vector<vector<double>> GenerateUnderlyingMatrix(double K, double sig, double r, double StartU, double b, double EndU, int steps);			// Underlying Price Varying Matrix Generator
vector<vector<double>> GenerateCostOfCarryMatrix(double K, double sig, double r, double U, double Start_b, double End_b, int steps);		// Cost of Carry Varying Matrix Generator
vector<vector<double>> MatrixPricer(vector<vector<double>> DataVec);				// Pricer of an input matrix

double CallPrice(double K, double sig, double r, double U, double b);				// Call price for a perpetual american option
double PutPrice(double K, double sig, double r, double U, double b);				// Put price for a perpetual american option

#endif