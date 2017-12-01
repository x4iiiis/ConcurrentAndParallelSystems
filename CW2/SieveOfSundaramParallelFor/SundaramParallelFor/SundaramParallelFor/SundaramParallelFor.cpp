// SundaramParallelFor.cpp : Defines the entry point for the console application.
//

// C++ program to print primes smaller than n using
// Sieve of Sundaram.
#include "stdafx.h"

//#include <bits/stdc++.h>

#include <iostream>
#include <fstream>
#include <chrono>

#include <thread>
#include <omp.h>

using namespace std;
using namespace chrono;

///////////////////////////////////////////////////////////////////////////////////////http://bcbutler.com/CPP_Tuts/c_plus_plus_sieve_of_sundaram.php
//////////////////////////////////////////////////////////////////////////////////////RUN ME IN X64
int main()
{
	int Runs = 1;
	int Max = 1000000000; // meant to be 1000000000

	auto threadCount = thread::hardware_concurrency();
	
	//R//Create time taken file
	ofstream timer("TimeTaken.csv", ofstream::out);					//Outside the loop so that it keeps track of previous runs 
	timer << "Milliseconds" << endl;

	for (int i = 0; i < Runs; i++)
	{
		cout << "Starting run " << i + 1 << " of " << Runs << endl;		//Will slightly affect performance but it allows to check the progress when running several times
																		//Won't affect time taken results though. 

		//R//Create results file
		ofstream results("PrimeNumbers.txt", ofstream::out);		//Inside the loop so that PrimeNumbers.txt is wiped each time and only keeps the prime numbers on the last run


		int totalPrimes = 0; // total number of prime numbers that are found
		int TheseArePrime = 0; // variable used in the array that stores the prime numbers found


							   // CALCULATIONS BELOW ARE TAKEN FROM:
							   // http://en.wikipedia.org/wiki/Sieve_of_Sundaram

		int n = Max / 2;

		int* isPrime = new int[Max]; //array to start off with that will eventually get all the composite numbers removed and the remaining ones output to the screen

		//  Fill the array with a list of integers up to the inputNumber  
		for (int i = 0; i < Max; i++)
		{
			isPrime[i] = i + 1;
		}


		//R//Timing the results
		auto start = system_clock::now();


		int j;
//#pragma omp parallel for num_threads(threadCount) private(j)
		for (int i = 1; i < n; i++)
		{
#pragma omp parallel for num_threads(threadCount)
			for (j = i; j <= (n - i) / (2 * i + 1); j++)
			{
				isPrime[i + j + 2 * i * j] = 0;/*From this list, remove all
											   numbers of the form i + j + 2ij    */
			}
		}
		if (Max >= 2)
		{
			isPrime[TheseArePrime++] = 2;/*this IF statement adds 2 to the output since 2 is a prime number    */
			totalPrimes++;
		}

		int R;
//#pragma omp parallel for num_threads(threadCount) private(R)
		for (R = 1; R < n; R++)
		{
			if (isPrime[R] != 0)
			{
				isPrime[TheseArePrime++] = R * 2 + 1;
				/*The remaining numbers are
				doubled and incremented by
				one, giving a list of the
				odd prime numbers (i.e., all
				primes except the only even
				prime 2) below 2n + 2.
				*/

				totalPrimes++;// the counter of the number of primes found
			}
		}

		/*  Output Prime Numbers */

		int x;
//#pragma omp parallel for num_threads(threadCount) private(x)
		for (x = 0; x < totalPrimes; x++)
		{
			if (isPrime[x] != 0)
			{
				results << isPrime[x] << endl;//outputs all prime numbers found
			}
			else
			{
				break;
			}
		}

		//R//Timing the results
		auto end = system_clock::now();
		auto total = duration_cast<milliseconds>(end - start).count();

		//R//Outputting the time taken to a text file
		timer << total << endl;

		delete[] isPrime;
	}
	return 0;
}