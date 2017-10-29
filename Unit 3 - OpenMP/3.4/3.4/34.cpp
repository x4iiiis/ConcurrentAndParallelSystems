// 3.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <omp.h>
#include <random>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

//Generates a vector of random values
vector<unsigned int> generate_values(unsigned int size)
{
	//Create random engine
	auto millis = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

	default_random_engine e(static_cast<unsigned int>(millis.count()));

	//Generate random numbers
	vector<unsigned int> data;

	for (unsigned int i = 0; i < size; i++)
	{
		data.push_back(e());

		return data;
	}
}


void parallel_sort(vector<unsigned int> &values)
{

	auto num_threads = thread::hardware_concurrency();
	// Get the number of elements in the vector
	auto n = values.size();

	int i, tmp, phase;

	// Declare parallel selection
#pragma omp parallel num_threads(num_threads) default(none) shared(values, n) private(i, tmp, phase)
	for (phase = 0; phase < n; phase++)
	{
		// Determine which phase of the sort we are in
		if (phase % 2 == 0)
		{
			// Parallel for loop. Each thread jumps forward 2, so no conflict
#pragma omp for
			for (i = 1; i < n; i += 2)
			{
				// Check if we should swap values
				if (values[i - 1] > values[i])
				{
					// Swap values
					tmp = values[i - 1];
					values[i - 1] = values[i];
					values[i] = tmp;
				}
			}
		}
		else
		{
			// Parallel for loop. Each thread jumps forward 2, so no conflict
#pragma omp for
			for (i = 1; i < n; i += 2)
			{
				// Check if we should swap values
				if (values[i] > values[i + 1])
				{
					// Swap values
					tmp = values[i + 1];
					values[i + 1] = values[i];
					values[i] = tmp;
				}
			}
		}
	}

}

int main()
{
	//Create results file
	ofstream results("parallel_sort.csv", ofstream::out);

	//Gather results for 2^8 to 2^16 results
	for (unsigned int size = 8; size <= 16; size++)
	{
		//output data size
		results << pow(2, size) << ", ";

		//Gather 100 results
		for (unsigned int i = 0; i < 100; i++)
		{
			//Generate vector of random values
			cout << "Generating " << i << " for " << pow(2, size) << " values" << endl;

			auto data = generate_values(static_cast<unsigned int>(pow(2, size)));

			//Sort the vector
			cout << "Sorting" << endl;
			auto start = chrono::system_clock::now();
			parallel_sort(data);
			auto end = chrono::system_clock::now();
			auto total = chrono::duration_cast<chrono::milliseconds>(end - start).count();

			//Output time
			results << total << " ,";
		}
		results << endl;
	}
	results.close();

	return 0;
}

