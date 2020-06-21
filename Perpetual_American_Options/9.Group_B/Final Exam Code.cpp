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
	// Initialize the UserChoice variable, which will be used later to store the user's chosen operation.
	int UserChoice = 0;
	// Initialize all the inputs needed for the Black-Scholes formula which will be used and modified by the user later.
	double K = 0.0, sig = 0.0, r = 0.0, U = 0.0, b = 0.0;

	// Ask the user whether they want to price a single option or see the price of an option over a range of values for one parameter while keeping all others constant.
	// Store user input in the UserChoice variable.
	cout << "What operation will be performed?:" << endl
		 << "1 for single option pricing" << endl
		 << "2 for varying parameter pricing" << endl;
	cin >> UserChoice;

	// Start switch statement based on UserChoice.
	switch (UserChoice)
	{
		// If the user input/UserChoice equals 1, start the single option pricing routine.
		case (1):
		{
			// Ask the user to input the parameters of the option they wish to price and store these parameters in their respective variables.
			cout << "Please input the parameters of your option in the following order, with each value separated by a space or new line: K, sig, r, U, b: " << endl;
			cin >> K >> sig >> r >> U >> b;

			// Create a PerpetualAmericanOption class object based on the parameters input by the user.
			PerpetualAmericanOption UserOpt(K, sig, r, U, b);

			// Calculate and output the prices for the Perpetual American Call and Put Options corresponding to the parameters given by the user.
			cout << endl << "CALL OPTION PRICE: C = " << UserOpt.Price() << endl;
			UserOpt.toggle();
			cout << "PUT OPTION PRICE: P = " << UserOpt.Price() << endl;

			return 0;
		}

		// If the user input/UserChoice equals 2, start the varying parameter pricing routine.
		case (2):
		{
			// Ask the user to input the parameters of the option they wish to price and store these parameters in their respective variables.
			cout << "Please input the initial parameters of your option in the following order, with each value separated by a space or new line: K, sig, r, U, b" << endl;
			cin >> K >> sig >> r >> U >> b;

			// Initialize the End_Val and Steps variables to hold the final value of the user's varying parameter and the steps the user wishes to take to get to that value.
			double End_Val = 0.0;
			int Steps = 0;

			// Ask the user to input the end value of the varying parameter and the number of steps the user wishes to take to get to that value.
			cout << "Please input the end value of the varying parameter and the number of steps to increment by in that order and separated by spaces or new lines: " << endl;
			cin >> End_Val >> Steps;

			// Initialize the VaryParam variable, which will be used later to store the variable the user wishes to have vary.
			int VaryParam = 0;

			// Ask the user to input the value corresponding to the variable they wish to vary.
			cout << "Which parameter will vary? (1 for K, 2 for sig, 3 for r, 4 for U, 5 for b): " << endl;
			cin >> VaryParam;

			// Start switch statement based on VaryParam.
			switch (VaryParam)
			{
				// If the user input / VaryParam equals 1, start the varying strike price (K) pricing routine.
				case (1):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different strike price corresponding to the initial strike price and the steps taken to arrive at the final strike price,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(K, sig, r, U, b, End_Val, Steps, Strike);
					// Call MatrixPricer to price the Perpetual American Option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange);

					// Print the header of the results table.
					cout << endl << "STRIKE PRICE | CALL PRICE | PUT PRICE " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the strike price, call price, and put price.
						cout << left << setw(13) << setfill(' ') << VaryRange[i][0]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1] << endl;

					}

					return 0;
				}

				// If the user input / VaryParam equals 2, start the varying volatility (sig) pricing routine.
				case (2):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different volatility corresponding to the initial volatility and the steps taken to arrive at the final volatility,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(K, sig, r, U, b, End_Val, Steps, Sigma);
					// Call MatrixPricer to price the Perpetual American Option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange);

					// Print the header of the results table.
					cout << endl << "VOLATILITY | CALL PRICE | PUT PRICE " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the volatility, call price, and put price.
						cout << left << setw(11) << setfill(' ') << VaryRange[i][1]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1] << endl;
					}

					return 0;
				}

				// If the user input / VaryParam equals 3, start the varying interest rate pricing routine.
				case (3):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different interest rate corresponding to the initial interest rate and the steps taken to arrive at the final interest rate,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(K, sig, r, U, b, End_Val, Steps, Interest);
					// Call MatrixPricer to price the Perpetual American Option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange);

					// Print the header of the results table.
					cout << endl << "INTEREST | CALL PRICE | PUT PRICE " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the interest rate, call price, and put price.
						cout << left << setw(9) << setfill(' ') << VaryRange[i][2]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1] << endl;
					}

					return 0;
				}

				// If the user input / VaryParam equals 4, start the varying underlying price pricing routine.
				case (4):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different underlying price corresponding to the initial underlying price and the steps taken to arrive at the final underlying price,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(K, sig, r, U, b, End_Val, Steps, Underlying);
					// Call MatrixPricer to price the Perpetual American Option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange);

					// Print the header of the results table.
					cout << endl << "UNDERLYING | CALL PRICE | PUT PRICE " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the underlying price, call price, and put price
						cout << left << setw(11) << setfill(' ') << VaryRange[i][3]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1] << endl;
					}

					return 0;
				}

				// If the user input / VaryParam equals 5, start the varying cost of carry pricing routine.
				case (5):
				{
					// Call GenerateParameterMatrix to generate a matrix where each row corresponds to a different cost of carry corresponding to the initial cost of carry and the steps taken to arrive at the final cost of carry,
					// holding all other parameters constant.
					vector<vector<double>> VaryRange = GenerateParameterMatrix(K, sig, r, U, b, End_Val, Steps, Cost_Of_Carry);
					// Call MatrixPricer to price the Perpetual American Option corresponding to the parameters specified in each row of the VaryRange matrix.
					vector<vector<double>> VaryPrice = MatrixPricer(VaryRange);

					// Print the header of the results table.
					cout << endl << "COST OF CARRY | CALL PRICE | PUT PRICE " << endl;
					// Loop through all the rows of VaryPrice.
					for (unsigned int i = 0; i < VaryPrice.size(); i++)
					{
						// Print the cost of carry, call price, and put price
						cout << left << setw(14) << setfill(' ') << VaryRange[i][4]
							<< "| " << left << setw(11) << setfill(' ') << VaryPrice[i][0]
							<< "| " << left << setw(10) << setfill(' ') << VaryPrice[i][1] << endl;
					}

					return 0;
				}

				// If the user didn't input a number from 1-5 for VaryParam, print an error message and end the program.
				default:
				{
					cout << endl << "No proper parameter was chosen. Try again later." << endl;
					return 0;
				}
			}
		}

		// If the user didn't input a number from 1-2 for UserChoice, print an error message and end the program.
		default:
		{
			cout << "No operation was chosen. Try again later.";
			return 0;
		}
	}
}