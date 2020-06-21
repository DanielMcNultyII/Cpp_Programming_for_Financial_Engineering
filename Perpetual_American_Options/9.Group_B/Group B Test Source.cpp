/*	Daniel McNulty II
*
*	'Group B Test Source.h"
*/

#include "Option.h"
#include "PerpetualAmericanOption.h"
#include <iostream>
#include <iomanip>
#include <vector>

/*
*	Cost of carry factor b must be included in formulae depending on the
*	derivative type. These are used in the generalised Black-Scholes formula.
*	If r is the risk-free interest and q is the continuous dividend yield then
*	the cost-of-carry b per derivative type is:
*
*	a) Black-Scholes (1973) stock option model: b = r
*	b) b = r - q Merton (1973) stock option model with continuous dividend yield
*	c) b = 0 Black (1976) futures option model
*	d) b = r - rf Garman and Kohlhagen (1983) currency option model, where rf is the
*	   'foreign' interest rate
*/

int main()
{
// GROUP B: Perpetual American Options
// b. Test the data with K = 100, sig = 0.1, r = 0.1, b =  0.02, S = 110
	PerpetualAmericanOption TestData(100, 0.1, 0.1, 110, 0.02);		// Create an PerpetualAmericanOption with K = 100, sig = 0.1, r = 0.1, b =  0.02, S = 110
	// Calculate and print the call and put option prices of the PerpetualAmericanOption TestData
	cout << "GROUP B: Perpetual American Options" << endl << "b. Test the data with K = 100, sig = 0.1, r = 0.1, b = 0.02, S = 110"<< endl <<"A perpetual American option with the above parameters has a call price of" << endl << "C = " << TestData.Price();
	TestData.toggle();				// Change TestData to a put option
	cout << endl << "and a put price of" << endl << "P = " << TestData.Price() << endl;
	TestData.toggle();				// Change TestData to a call option

// c. Use the code in part a) to compute the call and put option price for a monotonically increasing range of underlying values of S
	vector<double> UnderMesh = GenerateMeshArray(10, 50, 40);		// Generate a vector of monotonically increasing underlying values of S from 10 to 50
	vector<double> CallPrices;										// Vector to store calculated call prices
	vector<double> PutPrices;										// Vector to store calculated put prices
	for (unsigned int i = 0; i < UnderMesh.size(); i++)
	{
		CallPrices.push_back(TestData.PriceWithS(UnderMesh[i]));	// Calculate call prices for monotonically increasing underlying values of S using the TestData parameters and store it in vector CallPrices
		TestData.toggle();											// Change TestData to a put option
		PutPrices.push_back(TestData.PriceWithS(UnderMesh[i]));		// Calculate put prices for monotonically increasing underlying values of S using the TestData parameters and store it in vector PutPrices
		TestData.toggle();											// Change TestData to a call option
	}
	cout << endl << "c. Use the code in part a) to compute the call and put option price for a monotonically increasing range of underlying values of S" << endl << "Using the test parameters from part b. except for the S, for 10 <= S <= 50, yields" << endl << "UNDERLYING PRICE | CALL PRICE | PUT PRICE" << endl;
	for (unsigned int i = 0; i < CallPrices.size(); i++)
	{
		cout << left << setw(17) << setfill(' ') << UnderMesh[i] << "| " << left << setw(11) << setfill(' ') << CallPrices[i] << "| " << left << setw(9) << setfill(' ') << PutPrices[i] << endl;
	}

// d. Incorporate this into the matrix pricer code
	vector<vector<double>> ParametersWithVariedK = GenerateParameterMatrix(90, 0.1, 0.1, 110, 0.02, 110, 20, Strike);	// Generate matrix of perpetual American option parameters with varying strike prices
	vector<vector<double>> PricesVaryWithK = MatrixPricer(ParametersWithVariedK);										// Calculate the call and put prices of the matrix of perpetual American option parameters
	cout << endl << "d. Incorporate this into the matrix pricer code" << endl << "Using sig = 0.1, r = 0.1, S = 110, and b =  0.02, with 90 <= K <= 110 where K increments by 1, yields" << endl << "STRIKE (K) | CALL PRICE | PUT PRICE" << endl;
	for (unsigned int i = 0; i < PricesVaryWithK.size(); i++)
	{
		cout << left << setw(11) << setfill(' ') << ParametersWithVariedK[i][0] << "| " << left << setw(11) << setfill(' ') << PricesVaryWithK[i][0] << "| " << left << setw(9) << setfill(' ') << PricesVaryWithK[i][1] << endl;
	}
	vector<vector<double>> ParametersWithVariedSig = GenerateParameterMatrix(100, 0.05, 0.1, 110, 0.02, 0.15, 10, Sigma);	// Generate matrix of perpetual American option parameters with varying volatility
	vector<vector<double>> PricesVaryWithSig = MatrixPricer(ParametersWithVariedSig);										// Calculate the call and put prices of the matrix of perpetual American option parameters
	cout << endl << "Using K = 100, r = 0.1, S = 110, and b =  0.02, with 0.05 <= Volatility <= 0.15 where volatility increments by 0.01, yields" << endl << "VOLATILITY | CALL PRICE | PUT PRICE" << endl;
	for (unsigned int i = 0; i < PricesVaryWithSig.size(); i++)
	{
		cout << left << setw(11) << setfill(' ') << ParametersWithVariedSig[i][1] << "| " << left << setw(11) << setfill(' ') << PricesVaryWithSig[i][0] << "| " << left << setw(9) << setfill(' ') << PricesVaryWithSig[i][1] << endl;
	}
	vector<vector<double>> ParametersWithVariedR = GenerateParameterMatrix(100, 0.1, 0.05, 110, 0.02, 0.15, 10, Interest);	// Generate matrix of perpetual American option parameters with varying interest
	vector<vector<double>> PricesVaryWithR = MatrixPricer(ParametersWithVariedR);											// Calculate the call and put prices of the matrix of perpetual American option parameters
	cout << endl << "Using K = 100, sig = 0.1, S = 110, and b =  0.02, with 0.05 <= r <= 0.15 where r increments by 0.01, yields" << endl << "INTEREST (r) | CALL PRICE | PUT PRICE" << endl;
	for (unsigned int i = 0; i < PricesVaryWithR.size(); i++)
	{
		cout << left << setw(13) << setfill(' ') << ParametersWithVariedR[i][2] << "| " << left << setw(11) << setfill(' ') << PricesVaryWithR[i][0] << "| " << left << setw(9) << setfill(' ') << PricesVaryWithR[i][1] << endl;
	}
	vector<vector<double>> ParametersWithVariedS = GenerateParameterMatrix(100, 0.1, 0.1, 100, 0.02, 120, 10, Underlying);	// Generate matrix of perpetual American option parameters with varying underlying value
	vector<vector<double>> PricesVaryWithS = MatrixPricer(ParametersWithVariedS);											// Calculate the call and put prices of the matrix of perpetual American option parameters
	cout << endl << "Using K = 100, sig = 0.1, r = 0.1, and b =  0.02, with 100 <= S <= 120 where S increments by 2, yields" << endl << "UNDERLYING VALUE (S) | CALL PRICE | PUT PRICE" << endl;
	for (unsigned int i = 0; i < PricesVaryWithR.size(); i++)
	{
		cout << left << setw(21) << setfill(' ') << ParametersWithVariedS[i][3] << "| " << left << setw(11) << setfill(' ') << PricesVaryWithS[i][0] << "| " << left << setw(9) << setfill(' ') << PricesVaryWithS[i][1] << endl;
	}
	vector<vector<double>> ParametersWithVariedB = GenerateParameterMatrix(100, 0.1, 0.1, 110, 0.00, 0.1, 10, Cost_Of_Carry);	// Generate matrix of perpetual American option parameters with varying underlying value
	vector<vector<double>> PricesVaryWithB = MatrixPricer(ParametersWithVariedB);												// Calculate the call and put prices of the matrix of perpetual American option parameters
	cout << endl << "Using K = 100, sig = 0.1, r = 0.1, and S = 110, with 0.00 <= b <= 0.10 where b increments by 1, yields" << endl << "COST OF CARRY (b) | CALL PRICE | PUT PRICE" << endl;
	for (unsigned int i = 0; i < PricesVaryWithR.size(); i++)
	{
		cout << left << setw(18) << setfill(' ') << ParametersWithVariedB[i][4] << "| " << left << setw(11) << setfill(' ') << PricesVaryWithB[i][0] << "| " << left << setw(9) << setfill(' ') << PricesVaryWithB[i][1] << endl;
	}

	return 0;
}