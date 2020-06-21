/*	Daniel McNulty II
*
*	EuropeanOption.h
*/

#ifndef EuropeanOption_H
#define EuropeanOption_H

#include "Option.h"
#include <string>
#include <vector>
using namespace std;

class EuropeanOption : public Option
{
private:
	void Copy(const EuropeanOption& source);		// Copy the values of datamembers from one EuropeanOption object to another

public:
	// Data members for the variables involved in European option pricing
	double T;		// Expiry time
	double K;		// Strike price
	double sig;		// Volatility
	double r;		// Risk-free interest rate
	double U;		// Current price of the underlying security
	double b;		// Cost of carry

	// Constructors
	EuropeanOption();																				// Default constructor
	EuropeanOption(const EuropeanOption& source);													// Copy constructor
	EuropeanOption(const enum OptionType& optionType);												// Constructor that accepts an OptionType argument
	EuropeanOption(const struct EuroOptData& optionData);											// Constructor that accepts an EuropeanOptionData argument
	EuropeanOption(double T, double K, double sig, double r, double U, double b);					// Constructor that accepts EuropeanOption data member input
	EuropeanOption(double T, double K, double sig, double r, double U, double b, OptionType Opt);	// Constructor that accepts EuropeanOption and Option data member input
	// Destructors
	virtual ~EuropeanOption();			// Default destructor

	// Functions that calculate option price and sensitivities
	double Price() const;						// Calculate the price of the given option
	double Delta() const;						// Calculate the delta of the given option
	double Gamma() const;						// Calculate the gamma of the given option
	double Parity() const;						// Use parity to calculate price opposite to optionType
	double PriceWithS(double newU) const;		// Use underlying price as an argument to calculate option price
	double DeltaDiff(double h) const;			// Use divided differences to calculate delta
	double GammaDiff(double h) const;			// Use divided differences to calculate gamma

	// Assignment Operator
	EuropeanOption& operator = (const EuropeanOption& source);
};

struct EuroOptData	// A struct that contains all the variables involved in European option pricing
{
	double T;				// Expiry time
	double K;				// Strike price
	double sig;				// Volatility
	double r;				// Risk-free interest rate
	double U;				// Current price of the underlying security
	double b;				// Cost of carry
};

enum EuroOptParam			// enum to chose the varying parameter in the GenerateMatrix function
{
	Expiry,
	Strike,
	Sigma,
	Interest,
	Underlying,
	Cost_Of_Carry
};

enum PricerOutput			// enum to chose what you want output from the Matrix Pricer
{
	Price,
	Delta,
	Gamma
};

// Vector and Matrix Generators
vector<double> GenerateMeshArray(double begin, double end, int n);					// Mesh generator

vector<vector<double>> GenerateParameterMatrix(double T, double K, double sig, double r, double U, double b, double End_Parameter_Val, int steps, EuroOptParam VariedParameter);	// General Varying Parameter Matrix Generator
vector<vector<double>> GenerateExpiryMatrix(double StartT, double K, double sig, double r, double U, double b, double EndT, int steps);				// Expiry Time Varying Matrix Generator
vector<vector<double>> GenerateStrikeMatrix(double T, double StartK, double sig, double r, double U, double b, double EndK, int steps);				// Strike Price Varying Matrix Generator
vector<vector<double>> GenerateVolatilityMatrix(double T, double K, double Start_sig, double r, double U, double b, double End_sig, int steps);		// Volitility Varying Matrix Generator
vector<vector<double>> GenerateInterestMatrix(double T, double K, double sig, double Start_r, double U, double b, double End_r, int steps);			// Interest Varying Matrix Generator
vector<vector<double>> GenerateUnderlyingMatrix(double T, double K, double sig, double r, double StartU, double b, double EndU, int steps);			// Underlying Price Varying Matrix Generator
vector<vector<double>> GenerateCostOfCarryMatrix(double T, double K, double sig, double r, double U, double Start_b, double End_b, int steps);		// Cost of Cary Varying Matrix Generator

vector<vector<double>> MatrixPricer(const vector<vector<double>> DataVec, PricerOutput Out);			// General matrix pricer
vector<vector<double>> PriceVector(const vector<vector<double>> DataVec);			// Pricer of an input matrix
vector<vector<double>> DeltaVector(const vector<vector<double>> DataVec);			// Calculate the deltas of a matrix
vector<vector<double>> GammaVector(const vector<vector<double>> DataVec);			// Calculate the gammas of a matrix

// Call Option Global Functions
double CallPrice(double T, double K, double sig, double r, double U, double b);		// Price of call
double CallDelta(double T, double K, double sig, double r, double U, double b);		// Delta of call
double CallGamma(double T, double K, double sig, double r, double U, double b);		// Gamma of call

// Put Option Global Functions
double PutPrice(double T, double K, double sig, double r, double U, double b);		// Price of put
double PutDelta(double T, double K, double sig, double r, double U, double b);		// Delta of put
double PutGamma(double T, double K, double sig, double r, double U, double b);		// Gamma of put

// Divided Differences Functions
double CallDeltaDiff(double T, double K, double sig, double r, double U, double b, double h);			// Use divided differences to calculate call delta
double PutDeltaDiff(double T, double K, double sig, double r, double U, double b, double h);			// Use divided differences to calculate put delta
double CallGammaDiff(double T, double K, double sig, double r, double U, double b, double h);			// Use divided differences to calculate call gamma
double PutGammaDiff(double T, double K, double sig, double r, double U, double b, double h);			// Use divided differences to calculate put delta

// Put call parity functions
double CallToPut(double c, double T, double K, double r, double U);					// Use put-call parity to calculate put price
double PutToCall(double p, double T, double K, double r, double U);					// Use put-call parity to calculate put price
bool ParityCheck(double C, double P, double T, double K, double r, double U);		// Check if a call and put price pairing satisfies parity

#endif