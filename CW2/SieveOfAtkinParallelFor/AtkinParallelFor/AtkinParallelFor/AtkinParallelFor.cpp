// AtkinParallelFor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <omp.h>

#define ll long long 

//#include <bits/stdc++.h>

using namespace std;
using namespace chrono;


///////////////////////////////////////////////////////////////////////////////////////////////http://www.sanfoundry.com/cpp-program-implement-sieve-atkins/
auto sieve_atkins(ll int n)
{
	//R//Create results file
	ofstream results("PrimeNumbers.txt", ofstream::out);		//Inside the loop so that PrimeNumbers.txt is wiped each time and only keeps the prime numbers on the last run

																//R//Timing the results
	auto start = system_clock::now();

	vector<bool> is_prime(n + 1);
	is_prime[2] = true;
	is_prime[3] = true;
	for (ll int i = 5; i <= n; i++)
	{
		is_prime[i] = false;
	}
	ll int lim = ceil(sqrt(n));
	

#pragma parallel for thread_num(thread::hardware_concurrency())
	for (ll int x = 1; x <= lim; x++)
	{
//#pragma parallel for thread_num(thread::hardware_concurrency())
		for (ll int y = 1; y <= lim; y++)
		{
			ll int num = (4 * x * x + y * y);
			if (num <= n && (num % 12 == 1 || num % 12 == 5))
			{
				is_prime[num] = true;
			}
			num = (3 * x * x + y * y);
			if (num <= n && (num % 12 == 7))
			{
				is_prime[num] = true;
			}

			if (x > y)
			{
				num = (3 * x * x - y * y);
				if (num <= n && (num % 12 == 11))
				{
					is_prime[num] = true;
				}
			}
		}
	}


#pragma parallel for thread_num(thread::hardware_concurrency())
	for (ll int i = 5; i <= lim; i++)
	{
		if (is_prime[i])
		{
//#pragma parallel for thread_num(thread::hardware_concurrency())
			for (ll int j = i * i; j <= n; j += i)
			{
				is_prime[j] = false;
			}
		}
	}

	for (ll int i = 2; i <= n; i++)
	{
		if (is_prime[i])
		{
			results << i << endl;
		}
	}

	//R//Timing the results
	auto end = system_clock::now();
	auto total = duration_cast<milliseconds>(end - start).count();

	return total;
}

/*
* Main
*/
int main()
{
	int Runs = 10;
	ll int n = 1000000000;

	//R//Create time taken file
	ofstream timer("TimeTaken.csv", ofstream::out);					//Outside the loop so that it keeps track of previous runs 
	timer << "Milliseconds" << endl;

	for (int i = 0; i < Runs; i++)
	{
		cout << "Starting run " << i + 1 << " of " << Runs << endl;		//Will slightly affect performance but it allows to check the progress when running several times
																		//Won't affect time taken results though. 
		timer << sieve_atkins(n) << endl; //?
	}

	return 0;
}

