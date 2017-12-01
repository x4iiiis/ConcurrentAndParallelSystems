// SieveOfEratosthenesThreads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <chrono>

#include <thread>
#include <mutex>

using namespace std;
using namespace chrono;

mutex muteX;


void sieveOne(int upperBoundSquareRoot, bool *isComposite, int upperBound, ofstream results, int threadStart, int threadEnd)
{

	threadStart += 2;
	threadEnd += 2;

	//for (int m = 2; m <= upperBoundSquareRoot; m++)
	for (int m = threadStart; m <= threadEnd;)
	{
		if (!isComposite[m])
		{
			//cout << m << " ";			//Comment these out as they slow down performance
			muteX.lock();
			results << m << endl;		//R//Outputting the prime numbers to the text file
			muteX.unlock();

			for (int k = m * m; k <= upperBound; k += m)
			{
				isComposite[k] = true;
			}
		}
	}

	threadStart -= 2;
	threadEnd -= 2;

	//for (int m = upperBoundSquareRoot; m <= upperBound; m++)
	for (int m = threadStart; m <= threadEnd; m++)
	{
		if (!isComposite[m])
		{
			//cout << m << " ";			//Comment these out as they slow down performance
			muteX.lock();
			results << m << endl;		//R//Outputting the prime numbers to the text file
			muteX.unlock();
		}
	}

}



int main()
{
	///////////////////////////////////////////////////////////////////////////////////////http://www.algolist.net/Algorithms/Number_theoretic/Sieve_of_Eratosthenes
	int Runs = 10;
	int upperBound = 1000000000;	//Supposed to be 1 billion (1000000000)

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


		
		auto threadCount = thread::hardware_concurrency();
		vector<thread> threads;

		int threadStart, threadEnd;

		for (unsigned int t = 0; t < threadCount; t++)
		{
			int threadChunk = upperBoundSquareRoot / threadCount;
			threadStart = t * threadChunk;
			threadEnd = (t + 1) * threadChunk;
		}

		threads.push_back(thread(sieveOne, &upperBoundSquareRoot, &isComposite, &upperBound, &results, &threadStart, &threadEnd));
		
		//R//Timing the results
		auto start = system_clock::now();

		//for (int m = 2; m <= upperBoundSquareRoot; m++)
		//{
		//	if (!isComposite[m])
		//	{
		//		//cout << m << " ";			//Comment these out as they slow down performance
		//		results << m << endl;		//R//Outputting the prime numbers to the text file

		//		for (int k = m * m; k <= upperBound; k += m)
		//		{
		//			isComposite[k] = true;
		//		}
		//	}
		//}
		for (auto &t : threads)
		{
			t.join();
		}

		//for (int m = upperBoundSquareRoot; m <= upperBound; m++)
		//{
		//	if (!isComposite[m])
		//	{
		//		//cout << m << " ";			//Comment these out as they slow down performance
		//		results << m << endl;		//R//Outputting the prime numbers to the text file
		//	}
		//}

		//R//Timing the results
		auto end = system_clock::now();
		auto total = duration_cast<milliseconds>(end - start).count();

		//R//Outputting the time taken to a text file
		timer << total << endl;

		delete[] isComposite;
	}
	return 0;
}