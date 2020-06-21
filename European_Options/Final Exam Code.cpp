/*	Daniel McNulty II
*
*	Basic function to allow for user to choose function to perform and parameters to use made 
*	in order to make values easier to obtain on the final exam.
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
	// Initialize the UserChoice variable, which will be used later to store the user's chosen operation.
	int UserChoice = 0;
	// Initialize all the inputs needed for the Black-Scholes formula which will be used and modified by the user later.
	double T = 0.0, K = 0.0, sig = 0.0, r = 0.0, U = 0.0, b = 0.0;
	
	/*
	*	Ask the user whether they want to:
	*		1. Price one European option, as well as calculate its delta and gamma.
	*		2. Price a European option over a range of values for one parameter while keeping all other parameters constant.
	*		3. Check if Put-Call Parity is satisfied for a given call price and put price pair given specific parameters.
	*		4. Determine the price of a European Call Option given both the Put price of the option and the option parameters, or vice versa.
	*		5. Use single step divided differences to determine estimates for the delta and gamma of the European Call and Put options with the given parameters.
	*		6. Use multiple step divided differences to determine estimates for the delta and gamma of the European Call and Put options with the given parameters.
	*	Store user input in the UserChoice variable.
	*/
	cout << "What operation will be performed?:" << endl 
		 << "1 for single option pricing" << endl
		 << "2 for varying parameter pricing" << endl
		 << "3 for parity check" << endl
		 << "4 for pricing with parity" << endl
		 << "5 for single step divided differences" << endl
		 << "6 for multiple step divided differences" << endl;
	cin >> UserChoice;

	// Start switch statement based on UserChoice.
	switch (UserChoice)
	{
		// If the user input/UserChoice equals 1, start the single option pricing routine.
		case (1):
		{
			// Ask the user to input the parameters of the option they wish to price and store these parameters in their respective variables.
			cout << "Please input the parameters of your option in the following order, with each value separated by a space or new line: T, K, sig, r, U, b: " << endl;
			cin >> T >> K >> sig >> r >> U >> b;

			// Create a EuropeanOption class object based on the parameters input by the user.
			EuropeanOption UserOpt(T, K, sig, r, U, b);

			// Calculate and output the price, delta, and gamma for the European Call Option corresponding to the parameters given by the user.
			cout << endl << "CALL OPTION VALUES:" << endl 
				 << "Price = " << UserOpt.Price() << endl 
				 << "Delta = " << UserOpt.Delta() << endl 
				 << "Gamma = " << UserOpt.Gamma() << endl << endl;

			// Calculate and output the price, delta, and gamma for the European Put Option corresponding to the parameters given by the user.
			UserOpt.toggle();
			cout << "PUT OPTION VALUES:" << endl 
				 << "Price = " << UserOpt.Price() << endl 
				 << "Delta = " << UserOpt.Delta() << endl 
				 << "Gamma = " << UserOpt.Gamma() << endl << endl;
			
			return 0;
		}

		// If the user input/UserChoice equals 2, start the varying parameter pricing routine.
		case (2):
		{
			// Ask the user to input the parameters of the option they wish to price and store these parameters in their respective variables.
			cout << "Please input the initial parameters of your option in the following order, with each value separated by a space or new line: T, K, sig, r, U, b" << endl;
			cin >> T >> K >> sig >> r >> U >> b;

			// Initialize the End_Val and Steps variables to hold the final value of the user's varying parameter and the steps the user wishes to take to get to that value.
			double End_Val = 0.0;
			int Steps = 0;

			// Ask the user to input the end value of the varying parameter and the number of steps the user wishes to take to get to that value.
			cout << "Please input the end value of the varying parameter and the number of steps to increment by in that order and separated by spaces or new lines: " << endl;
			cin >> End_Val >> Steps;

			// Initialize the VaryParam variable, which will be used later to store the variable the user wishes to have vary.
			int VaryParam = 0;

			// Ask the user to input the value corresponding to the variable they wish to vary.
			cout << "Which parameter will vary? (1 for T, 2 for K, 3 for sig, 4 for r, 5 for U, 6 for b): " << endl;
			cin >> VaryParam;

			// Start switch statement based on VaryParam.
			switch (VaryParam)
			{
				// If the user input / VaryParam equals 1, start the varying expiry time pricing routine.
				case (1):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different expiry time corresponding to the initial expiry time and the steps taken to arrive at the final expiry time,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(T, K, sig, r, U, b, End_Val, Steps, Expiry);
					// Call MatrixPricer to determine the price, delta, and gamma of the European option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange, Price);
					vector<vector<double>> VaryDelta = MatrixPricer(VaryRange, Delta);
					vector<vector<double>> VaryGamma = MatrixPricer(VaryRange, Gamma);

					// Print the header of the results table.
					cout << endl << "EXPIRY TIME | CALL PRICE | PUT PRICE | CALL DELTA | PUT DELTA | CALL GAMMA | PUT GAMMA " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the expiry time, call price, put price, call delta, put delta, call gamma, and put gamma.
						cout << left << setw(12) << setfill(' ') << VaryRange[i][0]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryDelta[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryDelta[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryGamma[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryGamma[i][1] << endl;

					}

					return 0;
				}

				// If the user input / VaryParam equals 2, start the varying strike price pricing routine.
				case (2):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different strike price corresponding to the initial strike price and the steps taken to arrive at the final strike price,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(T, K, sig, r, U, b, End_Val, Steps, Strike);
					// Call MatrixPricer to determine the price, delta, and gamma of the European option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange, Price);
					vector<vector<double>> VaryDelta = MatrixPricer(VaryRange, Delta);
					vector<vector<double>> VaryGamma = MatrixPricer(VaryRange, Gamma);

					// Print the header of the results table.
					cout << endl << "STRIKE PRICE | CALL PRICE | PUT PRICE | CALL DELTA | PUT DELTA | CALL GAMMA | PUT GAMMA " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the strike price, call price, put price, call delta, put delta, call gamma, and put gamma.
						cout << left << setw(13) << setfill(' ') << VaryRange[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryDelta[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryDelta[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryGamma[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryGamma[i][1] << endl;

					}

					return 0;
				}

				// If the user input / VaryParam equals 3, start the varying volatility pricing routine.
				case (3):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different volatility corresponding to the initial volatility and the steps taken to arrive at the final volatility,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(T, K, sig, r, U, b, End_Val, Steps, Sigma);
					// Call MatrixPricer to determine the price, delta, and gamma of the European option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange, Price);
					vector<vector<double>> VaryDelta = MatrixPricer(VaryRange, Delta);
					vector<vector<double>> VaryGamma = MatrixPricer(VaryRange, Gamma);

					// Print the header of the results table.
					cout << endl << "VOLATILITY | CALL PRICE | PUT PRICE | CALL DELTA | PUT DELTA | CALL GAMMA | PUT GAMMA " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the volatility, call price, put price, call delta, put delta, call gamma, and put gamma.
						cout << left << setw(11) << setfill(' ') << VaryRange[i][2]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryDelta[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryDelta[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryGamma[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryGamma[i][1] << endl;
					}

					return 0;
				}

				// If the user input / VaryParam equals 4, start the varying interest rate pricing routine.
				case (4):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different interest rate corresponding to the initial interest rate and the steps taken to arrive at the final interest rate,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(T, K, sig, r, U, b, End_Val, Steps, Interest);
					// Call MatrixPricer to determine the price, delta, and gamma of the European option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange, Price);
					vector<vector<double>> VaryDelta = MatrixPricer(VaryRange, Delta);
					vector<vector<double>> VaryGamma = MatrixPricer(VaryRange, Gamma);

					// Print the header of the results table.
					cout << endl << "INTEREST | CALL PRICE | PUT PRICE | CALL DELTA | PUT DELTA | CALL GAMMA | PUT GAMMA " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the interest rate, call price, put price, call delta, put delta, call gamma, and put gamma.
						cout << left << setw(9) << setfill(' ') << VaryRange[i][3]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryDelta[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryDelta[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryGamma[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryGamma[i][1] << endl;
					}

					return 0;
				}

				// If the user input / VaryParam equals 5, start the varying underlying price pricing routine.
				case (5):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different underlying price corresponding to the initial underlying price and the steps taken to arrive at the final underlying price,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(T, K, sig, r, U, b, End_Val, Steps, Underlying);
					// Call MatrixPricer to determine the price, delta, and gamma of the European option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange, Price);
					vector<vector<double>> VaryDelta = MatrixPricer(VaryRange, Delta);
					vector<vector<double>> VaryGamma = MatrixPricer(VaryRange, Gamma);

					// Print the header of the results table.
					cout << endl << "UNDERLYING | CALL PRICE | PUT PRICE | CALL DELTA | PUT DELTA | CALL GAMMA | PUT GAMMA " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the interest rate, call price, put price, call delta, put delta, call gamma, and put gamma.
						cout << left << setw(11) << setfill(' ') << VaryRange[i][4]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryDelta[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryDelta[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryGamma[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryGamma[i][1] << endl;
					}

					return 0;
				}

				// If the user input / VaryParam equals 6, start the varying cost of carry pricing routine.
				case (6):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different cost of carry corresponding to the initial cost of carry and the steps taken to arrive at the final cost of carry,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(T, K, sig, r, U, b, End_Val, Steps, Cost_Of_Carry);
					// Call MatrixPricer to determine the price, delta, and gamma of the European option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange, Price);
					vector<vector<double>> VaryDelta = MatrixPricer(VaryRange, Delta);
					vector<vector<double>> VaryGamma = MatrixPricer(VaryRange, Gamma);

					// Print the header of the results table.
					cout << endl << "COST OF CARRY | CALL PRICE | PUT PRICE | CALL DELTA | PUT DELTA | CALL GAMMA | PUT GAMMA " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the interest rate, call price, put price, call delta, put delta, call gamma, and put gamma.
						cout << left << setw(14) << setfill(' ') << VaryRange[i][5]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryDelta[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryDelta[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryGamma[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryGamma[i][1] << endl;
					}

					return 0;
				}

				// If the user didn't input a number from 1-6 for VaryParam, print an error message and end the program.
				default:
				{
					cout << endl << "No proper parameter was chosen. Try again later." << endl;
					return 0;
				}
			}
		}

		// If the user input/UserChoice equals 3, start the Put-Call parity check routine.
		case (3):
		{
			// Initialize the variables C and P which will hold the user input European Call and Put price respectively.
			double C = 0, P = 0;
			// Ask users to input the European Call and Put prices in question, as well as the option parameters.
			cout << "Please input the call and put prices of your option, as well as parameters T, K, r, U in that order and with spaces or new lines separating the values: " << endl;
			cin >> C >> P >> T >> K >> r >> U;
			
			// Call the ParityCheck function to determine whether Put-Call parity holds for the input prices and parameters. Print the result.
			cout << "THE INPUT PARAMETERS " << (ParityCheck(C, P, T, K, r, U) ? "SATISFY PARITY" : "DO NOT SATISFY PARITY") << endl;

			return 0;
		}

		// If the user input/UserChoice equals 4, start the Put-Call parity pricing routine.
		case (4):
		{
			// Initialize the ParityType variable which will be used to hold the user's choice of performing either a Put-to-Call or Call-to-Put calculation.
			int ParityType;
			// Ask users to input the number corresponding to the type of pricing calculation they wish to perform.
			cout << "Are you performing a (1) Put-To-Call or (2) Call-To-Put calculation?: " << endl;
			cin >> ParityType;

			// Initialze KnownPrice variable which will hold the price the user currently knows/the price of the option type opposite the type the user wants the price of.
			double KnownPrice = 0.0;
			// Ask users to input the known option price, along with the parameters of the option.
			cout << "Please input the option price you currently have, T, K, r, and U in that order and separated by spaces or new lines: " << endl;
			cin >> KnownPrice >> T >> K >> r >> U;

			// If the user input/ParityType equals 1, call the PutToCall function to convert the known European Put price to the European Call price using Put-Call Parity and print the result.
			if (ParityType == 1)
			{
				cout << "The call price from the given put price and parameters is: " << endl 
					 << "C = " << PutToCall(KnownPrice, T, K, r, U) << endl;
				return 0;
			}
			// If the user input/ParityType equals 2, call the CallToPut function to convert the known European Call price to the European Put price using Put-Call Parity and print the result.
			else if (ParityType == 2)
			{
				cout << "The put price from the given call price and parameters is: " << endl 
					 << "P = " << CallToPut(KnownPrice, T, K, r, U) << endl;
				return 0;
			}
			// If the user didn't pick a number from 1-2, print an error message and end the program.
			else
			{
				cout << "No calculation was chosen. Try again later." << endl;
				return 0;
			}
		}

		// If the user input/UserChoice equals 5, start the single step divided differences delta and gamma pricing routine.
		case (5):
		{
			// Initialize variable h which will hold the step size the user wishes to use.
			double h;
			// Ask the user to input the parameters of the European option they wish to calculate delta and gamma for, as well as the step size they wish to use.
			cout << "Please input the parameters of your option in the following order, with each value separated by a space or new line: T, K, sig, r, U, b, h: " << endl;
			cin >> T >> K >> sig >> r >> U >> b >> h;

			// Initialize a EuropeanOption class object using the parameters the user input.
			EuropeanOption UserOpt(T, K, sig, r, U, b);

			// Calculate the European Call Option delta and gamma for the European Option generated by the user input parameters using a single step divided difference and print the result.
			cout << endl << "CALL DIVIDED DIFFERENCES VALUES:" << endl
				 << "Delta = " << UserOpt.DeltaDiff(h) << endl
				 << "Gamma = " << UserOpt.GammaDiff(h) << endl << endl;

			// Calculate the European Call Option delta and gamma for the European Option generated by the user input parameters using a single step divided difference and print the result.
			UserOpt.toggle();
			cout << "PUT DIVIDED DIFFERENCES VALUES:" << endl
				 << "Delta = " << UserOpt.DeltaDiff(h) << endl
				 << "Gamma = " << UserOpt.GammaDiff(h) << endl << endl;

			return 0;
		}

		// If the user input/UserChoice equals 6, start the multiple step divided differences delta and gamma pricing routine.
		case (6):
		{
			// Ask the user to input the parameters of the European option they wish to calculate delta and gamma for, as well as the step size they wish to use.
			cout << "Please input the parameters of your option in the following order, with each value separated by a space or new line: T, K, sig, r, U, b: " << endl;
			cin >> T >> K >> sig >> r >> U >> b;

			// Initialize a EuropeanOption class object using the parameters the user input.
			EuropeanOption UserOpt(T, K, sig, r, U, b);

			// Initialize variables h_start, h_end, which will hold the starting step size, ending step size, and number of steps to take to get from the start to the end respectively.
			double h_start = 0.0, h_end = 0.0;
			int steps = 0;
			// Ask the user to input the starting step size, ending step size, and number of steps to use.
			cout << "Please input your starting step size, ending step size, and number of steps in that order separated by a space or new line: " << endl;
			cin >> h_start >> h_end >> steps;

			// Generate a vector of different h values.
			vector<double> h_Varied = GenerateMeshArray(h_start, h_end, steps);
			// Print the header of the results table.
			cout << endl << "h        | CALL DELTA | CALL GAMMA | PUT DELTA | PUT GAMMA" << endl;
			for (unsigned int i = 0; i < h_Varied.size(); i++)
			{
				double DivDiffCallDel = UserOpt.DeltaDiff(h_Varied[i]);		// Calculate the divided differences value of call delta using the h referred to by h_Varied[i].
				double DivDiffCallGam = UserOpt.GammaDiff(h_Varied[i]);
				UserOpt.toggle();											// Change GammaExample to a put option.
				double DivDiffPutDel = UserOpt.DeltaDiff(h_Varied[i]);		// Calculate the divided differences value of call delta using the h referred to by h_Varied[i].
				double DivDiffPutGam = UserOpt.GammaDiff(h_Varied[i]);
				UserOpt.toggle();											// Change GammaExample to a call option.

				// Print the step size, call delta, call gamma, put delta, and put gamma.
				cout << left << setw(9) << setfill(' ') << h_Varied[i] << "| " 
					 << left << setw(11) << setfill(' ') << DivDiffCallDel << "| " 
					 << left << setw(11) << setfill(' ') << DivDiffCallGam << "| " 
					 << left << setw(10) << setfill(' ') << DivDiffPutDel << "| "
					 << left << setw(10) << setfill(' ') << DivDiffPutGam << endl;
			}

			return 0;
		}

		// If the user didn't input a number from 1-6 for UserChoice, print an error message and end the program.
		default:
		{
			cout << "No operation was chosen. Try again later.";
			return 0;
		}
	}
}