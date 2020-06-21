/*	Daniel McNulty II
*
*	EuropeanOption.cpp
*/

#include "EuropeanOption.h"
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <iostream>

using namespace boost::math;
using namespace std;

// PRIVATE MEMBER FUNCTIONS
void EuropeanOption::Copy(const EuropeanOption& source)	// Copy the values of datamembers from one EuropeanOption object to another
{
	Option::Copy(source);	// Call the base class Copy() to set the base class data member optionType equal to source.optionType
	T = source.T;			// Set the expiry time equal to the source's expiry time
	K = source.K;			// Set the strike price equal to the source's strike price
	sig = source.sig;		// Set the volatility equal to the source's volatility
	r = source.r;			// Set the risk-free interest rate equal to the source's risk-free interest rate
	U = source.U;			// Set the price of the underlying security to underlying security's price of the underlying security
	b = source.b;			// Set the cost of carry to the source's cost of carry
}

// PUBLIC MEMBER FUNCTIONS
// Constructors
EuropeanOption::EuropeanOption() : Option(), T(0.25), K(65.0), sig(0.30), r(0.08), U(60.0), b(0.08) {}	// Default constructor

EuropeanOption::EuropeanOption(const EuropeanOption& source) : Option(source), T(source.T), K(source.K), sig(source.sig), r(source.r), U(source.U), b(source.b) {}		// Copy constructor

EuropeanOption::EuropeanOption(const OptionType& optionType) : Option(optionType), T(0.25), K(65.0), sig(0.30), r(0.08), U(60.0), b(0.08) {}	// Constructor that accepts OptionType as an argument

EuropeanOption::EuropeanOption(const struct EuroOptData& optionData) : Option(), T(optionData.T), K(optionData.K), sig(optionData.sig), r(optionData.r), U(optionData.U), b(optionData.b) {}	// Constructor that accepts EuropeanOptionData

EuropeanOption::EuropeanOption(double T, double K, double sig, double r, double U, double b) : Option(), T(T), K(K), sig(sig), r(r), U(U), b(b) {}						// Constructor that accepts EuropeanOption data member input

EuropeanOption::EuropeanOption(double T, double K, double sig, double r, double U, double b, OptionType Opt) : Option(Opt), T(T), K(K), sig(sig), r(r), U(U), b(b) {}	// Constructor that accepts EuropeanOption and Option data member input

// Destructors
EuropeanOption::~EuropeanOption() {}	// Default destructor

// Functions that calculate option price and sensitivities
double EuropeanOption::Price() const
{
	if (optionType == Call)
		return ::CallPrice(T, K, sig, r, U, b);
	else
		return ::PutPrice(T, K, sig, r, U, b);
}

double EuropeanOption::Delta() const
{
	if (optionType == Call)
		return ::CallDelta(T, K, sig, r, U, b);
	else
		return ::PutDelta(T, K, sig, r, U, b);
}

double EuropeanOption::Gamma() const
{
	if (optionType == Call)
		return ::CallGamma(T, K, sig, r, U, b);
	else
		return ::PutGamma(T, K, sig, r, U, b);
}

double EuropeanOption::Parity() const			// Use put-call parity to calculate put price
{
	if (optionType == Call)
		return ::CallToPut(::CallPrice(T, K, sig, r, U, b), T, K, r, U);
	else
		return ::PutToCall(::PutPrice(T, K, sig, r, U, b), T, K, r, U);
}

double EuropeanOption::PriceWithS(double newU) const		// Use underlying price as an argument to calculate option price
{
	if (optionType == Call)
	{
		return ::CallPrice(T, K, sig, r, newU, b);
	}
	else
	{
		return ::PutPrice(T, K, sig, r, newU, b);
	}
}

double EuropeanOption::DeltaDiff(double h) const	// Use divided differences to calculate delta
{
	if (optionType == Call)
		return ::CallDeltaDiff(T, K, sig, r, U, b, h);
	else
		return ::PutDeltaDiff(T, K, sig, r, U, b, h);
}

double EuropeanOption::GammaDiff(double h) const	// Use divided differences to calculate gamma
{
	if (optionType == Call)
		return ::CallGammaDiff(T, K, sig, r, U, b, h);
	else
		return ::PutGammaDiff(T, K, sig, r, U, b, h);
}

// Assignment Operator
EuropeanOption& EuropeanOption::operator = (const EuropeanOption& source)
{
	if (this == &source)			// Avoid assigning to itself
	{
		return *this;
	}
	else
	{
		Copy(source);
		return *this;
	}
}

// GLOBAL EUROPEAN OPTION FUNCTIONS
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

vector<vector<double>> GenerateParameterMatrix(double T, double K, double sig, double r, double U, double b, double End_Parameter_Val, int steps, EuroOptParam VariedParameter)		// General Varying Parameter Matrix Generator
{
	switch (VariedParameter)
	{
	case (Expiry):
		return GenerateExpiryMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	case (Strike):
		return GenerateStrikeMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	case (Sigma):
		return GenerateVolatilityMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	case (Interest):
		return GenerateInterestMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	case (Underlying):
		return GenerateUnderlyingMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	case (Cost_Of_Carry):
		return GenerateCostOfCarryMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	default:
		cout << "ERROR: No proper parameter (Expiry, Strike, Sigma, Interest, Underlying, or Cost_Of_Carry) was chosen. Resorting to default parameter Underlying Price";
		return GenerateUnderlyingMatrix(T, K, sig, r, U, b, End_Parameter_Val, steps);
	}
}

vector<vector<double>> GenerateExpiryMatrix(double StartT, double K, double sig, double r, double U, double b, double EndT, int steps)			// Expiry Time Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;										// Create the return matrix
	vector<double> ExpiryTimeRange = GenerateMeshArray(StartT, EndT, steps);	// Generate the vector with the different Ts
	for (unsigned int i = 0; i <= steps; i++)									// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ ExpiryTimeRange[i], K, sig, r, U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateStrikeMatrix(double T, double StartK, double sig, double r, double U, double b, double EndK, int steps)				// Strike Price Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;										// Create the return matrix
	vector<double> StrikeRange = GenerateMeshArray(StartK, EndK, steps);		// Generate the vector with the different Ks
	for (unsigned int i = 0; i <= steps; i++)									// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ T, StrikeRange[i], sig, r, U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateVolatilityMatrix(double T, double K, double Start_sig, double r, double U, double b, double End_sig, int steps)	// Volatility Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> VolatilityRange = GenerateMeshArray(Start_sig, End_sig, steps);	// Generate the vector with the different volatilities
	for (unsigned int i = 0; i <= steps; i++)										// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ T, K, VolatilityRange[i], r, U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateInterestMatrix(double T, double K, double sig, double Start_r, double U, double b, double End_r, int steps)			// Interest Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;											// Create the return matrix
	vector<double> InterestRange = GenerateMeshArray(Start_r, End_r, steps);		// Generate the vector with the different rs
	for (unsigned int i = 0; i <= steps; i++)										// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ T, K, sig, InterestRange[i], U, b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateUnderlyingMatrix(double T, double K, double sig, double r, double StartU, double b, double EndU, int steps)			// Underlying Price Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;										// Create the return matrix
	vector<double> UnderRange = GenerateMeshArray(StartU, EndU, steps);			// Generate the vector with the different Us
	for (unsigned int i = 0; i <= steps; i++)									// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ T, K, sig, r, UnderRange[i], b });
	}

	return ReturnMatrix;
}

vector<vector<double>> GenerateCostOfCarryMatrix(double T, double K, double sig, double r, double U, double Start_b, double End_b, int steps)		// Cost of Cary Varying Matrix Generator
{
	vector<vector<double>> ReturnMatrix;										// Create the return matrix
	vector<double> bRange = GenerateMeshArray(Start_b, End_b, steps);			// Generate the vector with the different bs
	for (unsigned int i = 0; i <= steps; i++)									// Set the values within the return matrix
	{
		ReturnMatrix.push_back({ T, K, sig, r, U, bRange[i] });
	}

	return ReturnMatrix;
}

vector<vector<double>> MatrixPricer(const vector<vector<double>> DataVec, PricerOutput Out)			// General matrix pricer
{
	switch (Out)
	{
	case (Price):
		return PriceVector(DataVec);
	case (Delta):
		return DeltaVector(DataVec);
	case (Gamma):
		return GammaVector(DataVec);
	default:
		cout << "ERROR: No proper output (Price, Delta, or Gamma) was chosen. Resorting to default output Price";
		return PriceVector(DataVec);
	}
}

vector<vector<double>> PriceVector(const vector<vector<double>> DataVec)														// Return a price vector with a matrix of option parameters
{
	vector<vector<double>> Prices;																								// Create return price matrix
	for (unsigned int i = 0; i < DataVec.size(); i++)
	{
		double CallP = CallPrice(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4], DataVec[i][5]);		// Calculate call price
		double PutP = PutPrice(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4], DataVec[i][5]);		// Calculate put price

		Prices.push_back({ CallP, PutP });																						// Fill the return price matrix
	}

	return Prices;
}

vector<vector<double>> DeltaVector(const vector<vector<double>> DataVec)
{
	vector<vector<double>> Deltas;																								// Create return deltas matrix
	for (unsigned int i = 0; i < DataVec.size(); i++)
	{
		double CallD = CallDelta(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4], DataVec[i][5]);		// Calculate call delta price
		double PutD = PutDelta(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4], DataVec[i][5]);		// Calculate put delta price

		Deltas.push_back({ CallD, PutD });																						// Fill the return deltas matrix
	}

	return Deltas;
}

vector<vector<double>> GammaVector(const vector<vector<double>> DataVec)
{
	vector<vector<double>> Gammas;																								// Create return gammas matrix
	for (unsigned int i = 0; i < DataVec.size(); i++)
	{
		double CallG = CallGamma(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4], DataVec[i][5]);		// Calculate call gamma price
		double PutG = PutGamma(DataVec[i][0], DataVec[i][1], DataVec[i][2], DataVec[i][3], DataVec[i][4], DataVec[i][5]);		// Calculate put gamma price

		Gammas.push_back({ CallG, PutG });																						// Fill the return gamma matrix
	}

	return Gammas;
}

// Call Option Global Functions
double CallPrice(double T, double K, double sig, double r, double U, double b)		// Price of call
{
	double d1 = (log(U / K) + ((b + (pow(sig, 2) / 2)) * T)) / (sig * sqrt(T));
	double d2 = d1 - (sig * sqrt(T));
	normal_distribution<> N(0, 1);
	return (U * exp((b - r) * T) * cdf(N, d1)) - (K * exp(-r * T) * cdf(N, d2));
}

double CallDelta(double T, double K, double sig, double r, double U, double b)		// Delta of call
{
	double d1 = (log(U / K) + ((b + (pow(sig, 2) / 2)) * T)) / (sig * sqrt(T));
	normal_distribution<> N(0, 1);
	return exp((b - r) * T) * cdf(N, d1);
}

double CallGamma(double T, double K, double sig, double r, double U, double b)		// Gamma of call
{
	double d1 = (log(U / K) + ((b + (pow(sig, 2) / 2)) * T)) / (sig * sqrt(T));
	normal_distribution<> N(0, 1);
	return (pdf(N, d1) * exp((b - r) * T)) / (U * sig * sqrt(T));
}

// Put Option Global Functions
double PutPrice(double T, double K, double sig, double r, double U, double b)		// Price of put
{
	double d1 = (log(U / K) + ((b + (pow(sig, 2) / 2)) * T)) / (sig * sqrt(T));
	double d2 = d1 - (sig * sqrt(T));
	normal_distribution<> N(0, 1);
	return (K * exp(-r * T) * cdf(N, -d2)) - (U * exp((b - r) * T) * cdf(N, -d1));
}

double PutDelta(double T, double K, double sig, double r, double U, double b)		// Delta of put
{
	return CallDelta(T, K, sig, r, U, b) - exp((b - r) * T);
}

double PutGamma(double T, double K, double sig, double r, double U, double b)		// Gamma of put
{
	return CallGamma(T, K, sig, r, U, b);
}

// Divided Differences Functions
double CallDeltaDiff(double T, double K, double sig, double r, double U, double b, double h)			// Use divided differences to calculate call delta
{
	return (CallPrice(T, K, sig, r, U + h, b) - CallPrice(T, K, sig, r, U - h, b)) / (2 * h);
}

double PutDeltaDiff(double T, double K, double sig, double r, double U, double b, double h)				// Use divided differences to calculate put delta
{
	return (PutPrice(T, K, sig, r, U + h, b) - PutPrice(T, K, sig, r, U - h, b)) / (2 * h);
}

double CallGammaDiff(double T, double K, double sig, double r, double U, double b, double h)			// Use divided differences to calculate call gamma
{
	return (CallPrice(T, K, sig, r, U + h, b) - (2 * CallPrice(T, K, sig, r, U, b)) + CallPrice(T, K, sig, r, U - h, b)) / (pow(h, 2));
}

double PutGammaDiff(double T, double K, double sig, double r, double U, double b, double h)			// Use divided differences to calculate call gamma
{
	return (PutPrice(T, K, sig, r, U + h, b) - (2 * PutPrice(T, K, sig, r, U, b)) + PutPrice(T, K, sig, r, U - h, b)) / (pow(h, 2));
}

// Put call parity functions
double CallToPut(double c, double T, double K, double r, double U)				// Use put-call parity to calculate put price
{
	return c + (K * exp(-r * T)) - U;
}

double PutToCall(double p, double T, double K, double r, double U)				// Use put-call parity to calculate call price
{
	return p + U - (K * exp(-r * T));
}

bool ParityCheck(double C, double P, double T, double K, double r, double U)	// Check if a call and put price pairing satisfies parity
{
	double Epsilon = .0001;

	if ((abs(C - PutToCall(P, T, K, r, U)) < Epsilon) && (abs(P - CallToPut(C, T, K, r, U)) < Epsilon))
	{
		return true;
	}
	else
	{
		return false;
	}
}