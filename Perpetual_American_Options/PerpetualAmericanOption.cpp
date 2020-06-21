/*	Daniel McNulty II
*
*	PerpetualAmericanOption.cpp
*/

#include "PerpetualAmericanOption.h"
#include <string>
#include <vector>
using namespace std;

// Private Functions
void PerpetualAmericanOption::Copy(const PerpetualAmericanOption& Opt)		// Copy all values from another PerpetualAmericanOption object
{
	Option::Copy(Opt);
	K = Opt.K;
	sig = Opt.sig;
	r = Opt.r;
	U = Opt.U;
	b = Opt.b;
}

// Constructors
PerpetualAmericanOption::PerpetualAmericanOption() : Option(), K(100), sig(0.1), r(0.1), U(110), b(0.02) {}	// Default constructor

PerpetualAmericanOption::PerpetualAmericanOption(const PerpetualAmericanOption& Opt) : Option(Opt), K(Opt.K), sig(Opt.sig), r(Opt.r), U(Opt.U), b(Opt.b) {}			// Copy constructor

PerpetualAmericanOption::PerpetualAmericanOption(const enum OptionType type) : Option(type), K(100), sig(0.1), r(0.1), U(110), b(0.02) {}							// Constructor that accepts option type

PerpetualAmericanOption::PerpetualAmericanOption(double newK, double newSig, double newR, double newU, double newB) : Option(), K(newK), sig(newSig), r(newR), U(newU), b(newB) {}		// Constructor that accepts all derived class data

PerpetualAmericanOption::PerpetualAmericanOption(double newK, double newSig, double newR, double newU, double newB, const enum OptionType type) : Option(type), K(newK), sig(newSig), r(newR), U(newU), b(newB) {}		// Constructor that accepts all base and derived class data

// Destructors
PerpetualAmericanOption::~PerpetualAmericanOption() {}		// Default constructor

// Functionality
double PerpetualAmericanOption::Price() const				// Calculate the price of the given option
{
	if (optionType == Call)
		return ::CallPrice(K, sig, r, U, b);
	else
		return ::PutPrice(K, sig, r, U, b);
}

double PerpetualAmericanOption::PriceWithS(double newU) const		// Calculate price using the set K,sig, r, and b with a different underlying value
{
	if (optionType == Call)
	{
		return ::CallPrice(K, sig, r, newU, b);
	}
	else
	{
		return ::PutPrice(K, sig, r, newU, b);
	}
}
											
// Assignment operator
PerpetualAmericanOption& PerpetualAmericanOption::operator = (const PerpetualAmericanOption& Opt)
{
	if (this == &Opt)
	{
		return *this;
	}
	else
	{
		Option::operator = (Opt);
		Copy(Opt);
		return *this;
	}
}

// GLOBAL PERPETUAL AMERICAN OPTION FUNCTIONS
vector<double> GenerateMeshArray(double begin, double end, int steps)	// Mesh generator
{
	vector<double> MeshArray;
	double h = ((end - begin) / steps);
	for (int i = 0; i <= steps; i++)
	{
		MeshArray.push_back(begin + (h * i));
	}
	return MeshArray;
}


vector<vector<double>> GenerateParameterMatrix(double K, double sig, double r, double U, double b, double End_Parameter_Val, int steps, PerpAmerOptParam VariedParameter)					// General Parameter Varying Matrix Generator
{
	switch (VariedParameter)
	{
	case (Strike):
		return GenerateStrikeMatrix(K, sig, r, U, b, End_Parameter_Val, steps);
	case (Sigma):
		return GenerateVolatilityMatrix(K, sig, r, U, b, End_Parameter_Val, steps);
	case (Interest):
		return GenerateInterestMatrix(K, sig, r, U, b, End_Parameter_Val, steps);
	case (Underlying):
		return GenerateUnderlyingMatrix(K, sig, r, U, b, End_Parameter_Val, steps);
	case (Cost_Of_Carry):
		return GenerateCostOfCarryMatrix(K, sig, r, U, b, End_Parameter_Val, steps);
	default:
		cout << "ERROR: No proper parameter (Strike, Sigma, Interest, Underlying, or Cost_Of_Carry) was chosen. Resorting to default parameter Underlying Price";
		return GenerateUnderlyingMatrix(K, sig, r, U, b, End_Parameter_Val, steps);
	}
}

vector<vector<double>> GenerateStrikeMatrix(double StartK, double sig, double r, double U, double b, double EndK, int steps)				// Strike Price Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> StrikeRange = GenerateMeshArray(StartK, EndK, steps);			// Generate the vector with the different strike prices
	for (int i = 0; i <= steps; i++)												// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ StrikeRange[i], sig, r, U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateVolatilityMatrix(double K, double Start_sig, double r, double U, double b, double End_sig, int steps)	// Volatility Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> VolatilityRange = GenerateMeshArray(Start_sig, End_sig, steps);	// Generate the vector with the different volatilities
	for (int i = 0; i <= steps; i++)												// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ K, VolatilityRange[i], r, U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateInterestMatrix(double K, double sig, double Start_r, double U, double b, double End_r, int steps)
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> InterestRange = GenerateMeshArray(Start_r, End_r, steps);		// Generate the vector with the different interest rates
	for (int i = 0; i <= steps; i++)												// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ K, sig, InterestRange[i], U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateUnderlyingMatrix(double K, double sig, double r, double StartU, double b, double EndU, int steps)		// Underlying Price Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> UnderlyingRange = GenerateMeshArray(StartU, EndU, steps);	// Generate the vector with the different volatilities
	for (int i = 0; i <= steps; i++)												// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ K, sig, r, UnderlyingRange[i], b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateCostOfCarryMatrix(double K, double sig, double r, double U, double Start_b, double End_b, int steps)	// Cost of Carry Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> CostOfCarryRange = GenerateMeshArray(Start_b, End_b, steps);		// Generate the vector with the different volatilities
	for (int i = 0; i <= steps; i++)												// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ K, sig, r, U, CostOfCarryRange[i] });
	}

	return ReturnMatrix;
}

vector<vector<double>> MatrixPricer(const vector<vector<double>> DataVec)											// Return a price vector with a matrix of option parameters
{
	vector<vector<double>> Prices;																					// Create return price matrix
	for (unsigned int i = 0; i < DataVec.size(); i++)
	{
		double CallP = CallPrice(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4]);		// Calculate call price
		double PutP = PutPrice(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4]);			// Calculate put price

		Prices.push_back({ CallP, PutP });																			// Fill the return price matrix
	}

	return Prices;
}

double CallPrice(double K, double sig, double r, double U, double b)			// Call price for a perpetual american option
{
	double y1 = 0.5 - (b / pow(sig, 2)) + sqrt(pow((0.5 - (b / pow(sig, 2))), 2) + ((2 * r) / pow(sig, 2)));
	if ((y1 == 0.0) || (y1 == 1.0))
	{
		return U;
	}
	else
	{
		return ((K / (y1 - 1))*pow((((y1 - 1) / y1)*(U / K)), y1));
	}
}

double PutPrice(double K, double sig, double r, double U, double b)				// Put price for a perpetual american option
{
	double y2 = 0.5 - (b / pow(sig, 2)) - sqrt(pow((0.5 - (b / pow(sig, 2))), 2) + ((2 * r) / pow(sig, 2)));
	if ((y2 == 0.0) || (y2 == 1.0))
	{
		return U;
	}
	else
	{
		return ((K / (1 - y2))*pow((((y2 - 1) / y2)*(U / K)), y2));
	}
}