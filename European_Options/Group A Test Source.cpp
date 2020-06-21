/*	Daniel McNulty II
*
*	"Group A Test Source.cpp"
*/

#include "Option.h"
#include "EuropeanOption.h"
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

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
	// Create a vector of the data for the test batches given in the homework prompt, with contents ordered as (T, K, sig, r, S)
	vector<EuropeanOption> TestBatches(4);
	
	EuropeanOption Batch1(0.25, 65.0, 0.30, 0.08, 60.0, 0.08);			// Construct batch 1 European option using the EuropeanOption constructor which accepts all the different variables and uses the default option type (Call)
	EuropeanOption Batch2(1.0, 100.0, 0.2, 0.0, 100.0, 0.0, Call);		// Construct batch 2 European option using the EuropeanOption constructor which accepts all the different variables, including the option type 
	
	EuroOptData Batch3Data = { 1.0, 10.0, 0.50, 0.12, 5.0, 0.12 };		// Create a EuroOptData object for batch 3
	EuropeanOption Batch3(Batch3Data);									// Use the EuropeanOption constructor that accepts EuroOptData objects to construct a Batch 3 EuropeanOption object

	EuropeanOption Batch4(Call);										// Use the EuropeanOption constructor that accepts optionType to construct batch 4, then initialize the parameters
	Batch4.T = 30.0;
	Batch4.K = 100.0;
	Batch4.sig = 0.30;
	Batch4.r = 0.08;
	Batch4.U = 100.0;
	Batch4.b = Batch4.r;

	TestBatches = { Batch1, Batch2, Batch3, Batch4 };					// Place all the EuropeanOption objects into the vector TestBatches

// GROUP A: Exact Solutions of One-Factor Plain Options
// a. Implement the exact solution for European call and put options using the test batches using the Black-Scholes stock option model (b = r)
	cout << "GROUP A: Exact Solutions of One-Factor Plain Options" << endl << "a. Implement the exact solution for European call and put options using the test batches using the Black-Scholes stock option model (b = r)" << endl;

	// Vectors to hold the later calculated call and put options
	vector<double> CallPrices;
	vector<double> PutPrices;
	for (unsigned int i = 0; i < TestBatches.size(); i++)
	{
		// Calculate the call and put option prices for the batch refered to by TestBatches[i]
		double CallP = TestBatches[i].Price();					// Calculate the call price of the option referred to by TestBatches[i]
		TestBatches[i].toggle();								// Change the option referred to by TestBatches[i] to a put option
		double PutP = TestBatches[i].Price();					// Calculate the put price of the option referred to by TestBatches[i]
		TestBatches[i].toggle();								// Change the option referred to by TestBatches[i] back to a call option

		// Print the batch number and the results of the call and put option price calculation for TestBatches[i]
		cout << "Batch " << i + 1 << ":" << endl << "        Call Price: " << CallP << endl << "        Put Price:  " << PutP << endl;

		// Store the calculated call and put option prices for TestBatches[i] into vectors CallPrices and PutPrices respectively to be used later
		CallPrices.push_back(CallP);
		PutPrices.push_back(PutP);
	}

// b. Apply the put-call parity relationship to computer call and put option prices
	cout << endl << "b. Apply the put-call parity relationship to computer call and put option prices" << endl << "Checking parity for the call and put prices calculated using the exact solution" << endl;
	for (unsigned int i = 0; i < CallPrices.size(); i++)
	{
		// Print the batch number, call price, put price and whether the call and put prices satisfy put-call parity
		cout << "Batch " << i + 1 << ":" << endl << "        Call Price:   " << CallPrices[i] << endl << "        Put Price:    " << PutPrices[i] << endl << "        Parity Check: " << (ParityCheck(CallPrices[i], PutPrices[i], TestBatches[i].T, TestBatches[i].K, TestBatches[i].r, TestBatches[i].U) ? "SATISFIES PARITY" : "DOES NOT SATISFY PARITY") << endl;
	}
	cout << endl << "Given the call prices calculated with the exact solution, the put price for each batch according to put-call parity is" << endl;
	for (unsigned int i = 0; i < CallPrices.size(); i++)
	{
		// Use put-call parity to calculate the put price given the call price of the batch denoted by i
		double Put_From_Call = TestBatches[i].Parity();

		// Print the batch number, call price, and put price calculated by put-call parity
		cout << "Batch " << i + 1 << ":" << endl << "        Call Price from Exact Solution: " << CallPrices[i] << endl << "        Put Price from Put-Call Parity: " << Put_From_Call << endl;
	}
	cout << endl << "Given the put prices calculated with the exact solution, the call price for each batch according to put-call parity is" << endl;
	for (unsigned int i = 0; i < CallPrices.size(); i++)
	{
		// Use put-call parity to calculate the put price given the call price of the batch denoted by i
		double Call_From_Put = PutToCall(PutPrices[i], TestBatches[i].T, TestBatches[i].K, TestBatches[i].r, TestBatches[i].U);

		// Print the batch number, call price, and put price calculated by put-call parity
		cout << "Batch " << i + 1 << ":" << endl << "        Put Price from Exact Solution:   " << PutPrices[i] << endl << "        Call Price from Put-Call Parity: " << Call_From_Put << endl;
	}

// c. Compute option prices for a monotonically increasing range of underlying values of S
	cout << endl << "c. Compute option prices for a monotonically increasing range of underlying values of S" << endl;
	vector<double> UnderMesh = GenerateMeshArray(10, 50, 40);						// Create a vector of monotonically increasing underlying values of S
	vector<double> UnderCallPrices;
	vector<double> UnderPutPrices;
	// Generate EuropeanOption objects for batch 1
	EuropeanOption CopyBatch1(Batch1);									// Make a copy of batch 1 in order to show the copy constructor works
	// Calculate call and put prices for the monotonically increasing range of underlying values
	for (unsigned int i = 0; i < UnderMesh.size(); i++)
	{
		UnderCallPrices.push_back(CopyBatch1.PriceWithS(UnderMesh[i]));		// Call price calculation and storage
		CopyBatch1.toggle();												// Toggle CopyBatch1 to a put option
		UnderPutPrices.push_back(CopyBatch1.PriceWithS(UnderMesh[i]));		// Put price calculation and storage
		CopyBatch1.toggle();												// Toggle CopyBatch1 to a call option
	}
	cout << "Using a vector of monotonically increasing range of underlying values of S from 10 to 50, and the T, K, volatility, r, and b from Batch 1 (0.25, 65, 0.30, 0.08, and 0.08 respectively), yields: " << endl;
	cout << "UNDERLYING PRICE | CALL PRICE    | PUT PRICE" << endl;		// Heading for table of underlying, call, and put prices
	for (unsigned int i = 0; i < UnderMesh.size(); i++)
	{
		// Print the underlying price, call price, and put price
		cout << left << setw(17) << setfill(' ') << UnderMesh[i] << "| " << left << setw(14) << setfill(' ') << UnderCallPrices[i] << "| " << left << setw(9) << setfill(' ') << UnderPutPrices[i] << endl;
	}

// d. Compute option prices as a function of i) expiry time, ii) volatility, or iii) any parameter
	cout << endl << "d. Compute option prices as a function of a varying parameter" << endl << "Using Batch 1 data of K = 65, sig = 0.30, b = r = 0.08, S = 60 for a range of 0.2 <= T <= 0.3 with .01 increments yields" << endl << "EXPIRY TIME | CALL PRICE | PUT PRICE" << endl;
	vector<vector<double>> ExpiryTimeRange = GenerateParameterMatrix(0.2, 65.0, 0.30, 0.08, 60.0, 0.08, 0.3, 10, Expiry);
	vector<vector<double>> ExpiryTimeRangePrice = MatrixPricer(ExpiryTimeRange, Price);
	for (unsigned int i = 0; i < ExpiryTimeRangePrice.size(); i++)
	{
		// Print the expiry time, call price, and put price
		cout << left << setw(12) << setfill(' ') << ExpiryTimeRange[i][0] << "| " << left << setw(11) << setfill(' ') << ExpiryTimeRangePrice[i][0] << "| " << left << setw(9) << setfill(' ') << ExpiryTimeRangePrice[i][1] << endl;
	}

	cout << endl << "Using Batch 1 data of T = 0.25, K = 65, b = r = 0.08, S = 60 for a range of 0.25 <= sig <= 0.35 with .01 increments yields" << endl << "VOLATILITY | CALL PRICE | PUT PRICE" << endl;
	vector<vector<double>> VolatilityRange = GenerateParameterMatrix(0.25, 65.0, 0.25, 0.08, 60.0, 0.08, 0.35, 10, Sigma);
	vector<vector<double>> VolatilityRangePrice = MatrixPricer(VolatilityRange, Price);
	for (unsigned int i = 0; i < VolatilityRangePrice.size(); i++)
	{
		// Print the volatility, call price, and put price
		cout << left << setw(11) << setfill(' ') << VolatilityRange[i][2] << "| " << left << setw(11) << setfill(' ') << VolatilityRangePrice[i][0] << "| " << left << setw(9) << setfill(' ') << VolatilityRangePrice[i][1] << endl;
	}

	cout << endl << "Using Batch 1 data of T = 0.25, K = 65, sig = 30, b = 0.08, S = 60 for a range of 0.00 <= r <= 0.10 with .01 increments yields" << endl << "INTEREST | CALL PRICE | PUT PRICE" << endl;
	vector<vector<double>> InterestRange = GenerateParameterMatrix(0.25, 65.0, 0.30, 0.00, 60.0, 0.08, 0.10, 10, Interest);
	vector<vector<double>> InterestRangePrice = MatrixPricer(InterestRange, Price);
	for (unsigned int i = 0; i < InterestRangePrice.size(); i++)
	{
		// Print the interest, call price, and put price
		cout << left << setw(9) << setfill(' ') << InterestRange[i][3] << "| " << left << setw(11) << setfill(' ') << InterestRangePrice[i][0] << "| " << left << setw(9) << setfill(' ') << InterestRangePrice[i][1] << endl;
	}

// GROUP A: Option Sensitivities, aka the Greeks
// a. Implement the formulae for gamma for call and put future option pricing
	cout << endl << endl << "GROUP A: Option Sensitivities, aka the Greeks" << endl << "a. Implement the formulae for delta and gamma for call and put future option pricing using K = 100, S = 105, T = 0.5, r = 0.1, b = 0, and sig = 0.36" << endl;
	EuropeanOption GammaExample(0.5, 100.0, 0.36, 0.1, 105.0, 0);																			// Create a European call option from the data given for this problem
	cout << "For the given example, delta call is " << GammaExample.Delta() << " and gamma call is " << GammaExample.Gamma() << endl;		// Calculate and print the call gamma for GammaExample
	GammaExample.toggle();																													// Convert the call option GammaExample to a put option
	cout << "For the given example, delta put is " << GammaExample.Delta() << " and gamma put is " << GammaExample.Gamma() << endl;			// Calculate and print the put gamma for GammaExample

// b. Use the code in part a to compute call delta price for a monotonically increasing range of underlying values of S
	vector<vector<double>> DeltaVaryWithS;
	for (unsigned int i = 0; i < UnderMesh.size(); i++)
	{
		GammaExample.U = UnderMesh[i];						// Set the underlying value of S equal to that referred to be UnderMesh[i]
		double CallD = GammaExample.Delta();				// Calculate the call delta
		GammaExample.toggle();								// Change GammaExample to a put option
		double PutD = GammaExample.Delta();					// Calculate put delta
		DeltaVaryWithS.push_back({ CallD, PutD });			// Store the call and put deltas in vector DeltaVaryWithS
		GammaExample.toggle();								// Change GammaExample back to a call option
	}
	cout << endl << "b. Use the code in part a to compute call delta price for a monotonically increasing range of underlying values of S" << endl << "For S monotonically increasing from 10 to 50 using K = 100, T = 0.5, r = 0.1, b = 0, and sig = 0.36, delta values are as follows" << endl << "UNDERLYING VALUE | CALL DELTA | PUT DELTA" << endl;
	for (unsigned int i = 0; i < DeltaVaryWithS.size(); i++)
	{
		// Print the underlying price, call delta, and put delta
		cout << left << setw(17) << setfill(' ') << UnderMesh[i] << "| " << left << setw(11) << setfill(' ') << DeltaVaryWithS[i][0] << "| " << left << setw(9) << setfill(' ') << DeltaVaryWithS[i][1] << endl;
	}

// c. Incorporate this into your matrix pricer code
	cout << endl << "c. Incorporate this into your matrix pricer code" << endl << "Using K = 100, S = 105, r = 0.1, b = 0, and sig = 0.36, as well as 0 <= T <= 1 where T increases in .1 increments, yields" << endl << "EXPIRY TIME | CALL DELTA | PUT DELTA" << endl;
	vector<vector<double>> ExpiryTimeRangeForDelta = GenerateParameterMatrix(0.0, 100.0, 0.36, 0.1, 105.0, 0.0, 1.0, 10, Expiry);
	vector<vector<double>> DeltaVaryWithT = MatrixPricer(ExpiryTimeRangeForDelta, Delta);
	for (unsigned int i = 0; i < DeltaVaryWithT.size(); i++)
	{
		// Print the volatility, call price, and put price
		cout << left << setw(12) << setfill(' ') << ExpiryTimeRangeForDelta[i][0] << "| " << left << setw(11) << setfill(' ') << DeltaVaryWithT[i][0] << "| " << left << setw(9) << setfill(' ') << DeltaVaryWithT[i][1] << endl;
	}

	cout << endl << "Using T = 0.5, K = 100, S = 105, r = 0.1, and b = 0, as well as 0.3 <= sig <= 0.4 where sig increases in .01 increments, yields" << endl << "VOLATILITY | CALL GAMMA | PUT GAMMA" << endl;
	vector<vector<double>> VolatilityRangeForGamma = GenerateParameterMatrix(0.5, 100.0, 0.3, 0.1, 105.0, 0.0, .4, 10, Sigma);
	vector<vector<double>> GammaVaryWithSig = MatrixPricer(VolatilityRangeForGamma, Gamma);
	for (unsigned int i = 0; i < DeltaVaryWithT.size(); i++)
	{
		// Print the volatility, call price, and put price
		cout << left << setw(11) << setfill(' ') << VolatilityRangeForGamma[i][2] << "| " << left << setw(11) << setfill(' ') << GammaVaryWithSig[i][0] << "| " << left << setw(9) << setfill(' ') << GammaVaryWithSig[i][1] << endl;
	}

// d. Perform the same calculations for parts a and b using divided differences
	cout << endl << "d. Perform the same calculations as parts a and b using divided differences" << endl << "Implement the formulae for delta and gamma for call and put future option pricing using K = 100, S = 105, T = 0.5, r = 0.1, b = 0, and sig = 0.36" << endl << "h        | CALL DELTA | PUT DELTA | CALL ERROR | PUT ERROR" << endl;
	vector<double> h_Varied = GenerateMeshArray(0.00000001, 1, 20);		// Generate an vector of different h values
	GammaExample.U = 105.0;												// Set GammaExample's underlying stock price back to it's default value
	double ExactPutDelta = GammaExample.Delta();						// Get exact solution of call delta for the example
	GammaExample.toggle();
	double ExactCallDelta = GammaExample.Delta();						// Get exact solution of put delta for the example
	for (unsigned int i = 0; i < h_Varied.size(); i++)
	{
		double DivDiffCallDel = GammaExample.DeltaDiff(h_Varied[i]);	// Calculate the divided differences value of call delta using the h referred to by h_Varied[i]
		GammaExample.toggle();											// Change GammaExample to a put option
		double DivDiffPutDel = GammaExample.DeltaDiff(h_Varied[i]);		// Calculate the divided differences value of call delta using the h referred to by h_Varied[i]
		GammaExample.toggle();											// Change GammaExample to a call option

		cout << left << setw(9) << setfill(' ') << h_Varied[i] << "| " << left << setw(11) << setfill(' ') << DivDiffCallDel << "| " << left << setw(10) << setfill(' ') << DivDiffPutDel << "| " << left << setw(11) << setfill(' ') << abs(DivDiffCallDel - ExactCallDelta) << "| " << left << setw(9) << setfill(' ') << abs(DivDiffPutDel - ExactPutDelta) << endl;
	}
	double ExactPutGamma = GammaExample.Gamma();						// Get exact solution of call gamma for the example
	GammaExample.toggle();
	double ExactCallGamma = GammaExample.Gamma();						// Get exact solution of put gamma for the example
	GammaExample.toggle();
	cout << endl << "h        | CALL GAMMA | PUT GAMMA | CALL ERROR | PUT ERROR" << endl;
	for (unsigned int i = 0; i < h_Varied.size(); i++)
	{
		double DivDiffCallGam = GammaExample.GammaDiff(h_Varied[i]);	// Calculate the divided differences value of call gamma using the h referred to by h_Varied[i]
		GammaExample.toggle();											// Change GammaExample to a put option
		double DivDiffPutGam = GammaExample.GammaDiff(h_Varied[i]);		// Calculate the divided differences value of call gamma using the h referred to by h_Varied[i]
		GammaExample.toggle();											// Change GammaExample to a call option

		cout << left << setw(9) << setfill(' ') << h_Varied[i] << "| " << left << setw(11) << setfill(' ') << DivDiffCallGam << "| " << left << setw(10) << setfill(' ') << DivDiffPutGam << "| " << left << setw(11) << setfill(' ') << abs(DivDiffCallGam - ExactCallGamma) << "| " << left << setw(9) << setfill(' ') << abs(DivDiffPutGam - ExactPutGamma) << endl;
	}
	return 0;
}