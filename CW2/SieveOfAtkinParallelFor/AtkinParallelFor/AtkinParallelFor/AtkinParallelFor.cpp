// AtkinParallelFor.cpp : Defines the entry point for the console application.
//
// SieveOfAtkinSequential.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <chrono>

#include <thread>
#include <omp.h>

//#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

///////////////////////////////////////////////////////////////////////////////////////http://www.geeksforgeeks.org/sieve-of-atkin/
auto SieveOfAtkin(int limit)
{
	auto threadCount = thread::hardware_concurrency();

	//R//Create results file
	ofstream results("PrimeNumbers.txt", ofstream::out);		//Inside the loop so that PrimeNumbers.txt is wiped each time and only keeps the prime numbers on the last run

																// 2 and 3 are known to be prime
	if (limit > 2)  results << 2 << endl;					//*********************************************************NOTICE THESE************************************************
	if (limit > 3)  results << 3 << endl;					//*********************************************************NOTICE THESE************************************************

															// Initialise the sieve array with false values
	bool* sieve = new bool[limit];

	int i;
#pragma omp parallel for num_threads(threadCount) private(i)
	for (i = 0; i < limit; i++)
	{
		sieve[i] = false;
	}

	//R//Timing the results
	auto start = system_clock::now();


	int n;
#pragma omp parallel for num_threads(threadCount) private(n)

	/* Mark siev[n] is true if one of the following is true:
	a) n = (4*x*x)+(y*y) has odd number of solutions, i.e., there exist
	odd number of distinct pairs (x, y) that satisfy the equation and
	n % 12 = 1 or n % 12 = 5.
	b) n = (3*x*x)+(y*y) has odd number of solutions and n % 12 = 7
	c) n = (3*x*x)-(y*y) has odd number of solutions, x > y and n % 12 = 11 */
	for (int x = 1; x*x < limit; x++)
	{
		for (int y = 1; y*y < limit; y++)
		{
			// Main part of Sieve of Atkin
			n = (4 * x*x) + (y*y);
			if (n <= limit && (n % 12 == 1 || n % 12 == 5))
				sieve[n] ^= true;

			n = (3 * x*x) + (y*y);
			if (n <= limit && n % 12 == 7)
				sieve[n] ^= true;

			n = (3 * x*x) - (y*y);
			if (x > y && n <= limit && n % 12 == 11)
				sieve[n] ^= true;
		}
	}

	//int i;
	int r;
#pragma omp parallel for num_threads(threadCount) private(i)

	// Mark all multiples of squares as non-prime
	for (r = 5; r*r < limit; r++)
	{
		if (sieve[r])
		{
			for (i = r*r; i < limit; i += r*r)
				sieve[i] = false;
		}
	}

	int a;
//#pragma omp parallel for num_threads(threadCount) private (a) -------------------- Don't want this as it messes with output order

	// Print primes using sieve[]
	for (a = 5; a < limit; a++)
	{
		if (sieve[a])
		{
			//cout << a << " ";			//Comment these out as they slow down performance
			results << a << endl;		//R//Outputting the prime numbers to the text file
		}
	}
	//R//Timing the results
	auto end = system_clock::now();
	auto total = duration_cast<milliseconds>(end - start).count();

	//R//Outputting the time taken to a text file
	//timer << total << endl;
	delete[] sieve;

	return total;
}

//Driver program
int main(void)
{
	int Runs = 1;
	int limit = 1000000000; //Supposed to be 1 billion (1000000000)

							//R//Create time taken file
	ofstream timer("TimeTaken.csv", ofstream::out);					//Outside the loop so that it keeps track of previous runs 
	timer << "Milliseconds" << endl;

	for (int i = 0; i < Runs; i++)
	{
		cout << "Starting run " << i + 1 << " of " << Runs << endl;		//Will slightly affect performance but it allows to check the progress when running several times
																		//Won't affect time taken results though. 
		timer << SieveOfAtkin(limit) << endl; //?
	}
	return 0;
}

