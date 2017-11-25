// SieveOfSundaramSequent.cpp : Defines the entry point for the console application.
//

// C++ program to print primes smaller than n using
// Sieve of Sundaram.
#include "stdafx.h"

//#include <bits/stdc++.h>

#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

// Prints all prime numbers smaller
auto SieveOfSundaram(int n)
{
	//R//Create results file
	ofstream results("PrimeNumbers.txt", ofstream::out);		//Inside the loop so that PrimeNumbers.txt is wiped each time and only keeps the prime numbers on the last run

																// In general Sieve of Sundaram, produces primes smaller
																// than (2*x + 2) for a number given number x.
																// Since we want primes smaller than n, we reduce n to half
	int nNew = (n - 2) / 2;

	// This array is used to separate numbers of the form i+j+2ij
	// from others where  1 <= i <= j
	bool* marked = new bool[nNew + 1];

	// Initalize all elements as not marked
	memset(marked, false, sizeof(marked));


	//R//Timing the results
	auto start = system_clock::now();


	// Main logic of Sundaram.  Mark all numbers of the
	// form i + j + 2ij as true where 1 <= i <= j
	for (int i = 1; i <= nNew; i++)
	{
		for (int j = i; (i + j + 2 * i*j) <= nNew; j++)
		{
			marked[i + j + 2 * i*j] = true;
		}
	}
	// Since 2 is a prime number
	if (n > 2)
	{
		results << 2 << endl;					//*********************************************************NOTICE THESE************************************************
	}

	// Print other primes. Remaining primes are of the form
	// 2*i + 1 such that marked[i] is false.
	for (int i = 1; i <= nNew; i++)
	{
		if (marked[i] == false)
		{
			results << 2 * i + 1 << endl;
		}
	}
	//R//Timing the results
	auto end = system_clock::now();
	auto total = duration_cast<milliseconds>(end - start).count();

	delete[] marked;

	return total;
}

// Driver program to test above
int main(void)
{
	int Runs = 10;
	int n = 1000;//000000; //Supposed to be 1 billion (1000000000)

				 //R//Create time taken file
	ofstream timer("TimeTaken.csv", ofstream::out);					//Outside the loop so that it keeps track of previous runs 
	timer << "Milliseconds" << endl;

	for (int i = 0; i < Runs; i++)
	{
		cout << "Starting run " << i + 1 << " of " << Runs << endl;		//Will slightly affect performance but it allows to check the progress when running several times
																		//Won't affect time taken results though. 
		timer << SieveOfSundaram(n) << endl; //?
	}
	return 0;
}

