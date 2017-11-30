// EratosthenesParallelFor.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <chrono>

#include <thread>
#include <omp.h>



using namespace std;
using namespace chrono;

int main()
{
	///////////////////////////////////////////////////////////////////////////////////////http://www.algolist.net/Algorithms/Number_theoretic/Sieve_of_Eratosthenes
	int Runs = 10;
	int upperBound = 1000000000;	//Supposed to be 1 billion (1000000000)

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

		int upperBoundSquareRoot = (int)sqrt((double)upperBound);
		bool *isComposite = new bool[upperBound + 1];
		memset(isComposite, 0, sizeof(bool) * (upperBound + 1));


		//R//Timing the results
		auto start = system_clock::now();

		int m;
#pragma omp parallel for num_threads(threadCount) private (m)

		for (m = 2; m <= upperBoundSquareRoot; m++)
		{
			if (!isComposite[m])
			{
				//cout << m << " ";			//Comment these out as they slow down performance
				results << m << endl;		//R//Outputting the prime numbers to the text file

				for (int k = m * m; k <= upperBound; k += m)
				{
					isComposite[k] = true;
				}
			}
		}
		
#pragma omp parallel for num_threads(threadCount) private (m)
		for ( m = upperBoundSquareRoot; m <= upperBound; m++)
		{
			if (!isComposite[m])
			{
				//cout << m << " ";			//Comment these out as they slow down performance
				results << m << endl;		//R//Outputting the prime numbers to the text file
			}
		}

		//R//Timing the results
		auto end = system_clock::now();
		auto total = duration_cast<milliseconds>(end - start).count();

		//R//Outputting the time taken to a text file
		timer << total << endl;

		delete[] isComposite;
	}
	return 0;
}

