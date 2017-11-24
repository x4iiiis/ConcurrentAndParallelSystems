// Programiz.cpp : Defines the entry point for the console application.
//https://www.programiz.com/cpp-programming/examples/prime-number

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

int main()
{
	//R//
	int Runs = 10;
	int max = 1000000000; //Supposed to be 1000000000
	
	//R//Create time taken file
	ofstream timer("TimeTaken.csv", ofstream::out);					//Outside the loop so that it keeps track of previous runs 
	timer << "Milliseconds" << endl;

	for (int R = 0; R < Runs; R++)
	{
		cout << "Starting run " << R + 1 << " of " << Runs << endl;		//Will slightly affect performance but it allows to check the progress when running several times
																		//Won't affect time taken results though.

		//R//Create results file
		ofstream results("PrimeNumbers.txt", ofstream::out);		//Inside the loop so that PrimeNumbers.txt is wiped each time and only keeps the prime numbers on the last run

		
		int n = 0;
		int i = 2;
		bool isPrime = true;

		//R//Timing the results
		auto start = system_clock::now();

		for (int R = 2; R < max; R++)
		{
			n = R;

			for (i = 2; i <= n / 2; ++i)
			{
				if (n % i == 0)
				{
					isPrime = false;
					break;
				}
			}
			if (isPrime)
			{
				//cout << R << " is a prime number" << endl;
				results << R << endl;
			}

			//R//Reset it for the loop 
			isPrime = true;
		}
		//R//Timing the results
		auto end = system_clock::now();
		auto total = duration_cast<milliseconds>(end - start).count();

		//R//Outputting the time taken to a text file
		timer << total << endl;
	}
	return 0;
}

