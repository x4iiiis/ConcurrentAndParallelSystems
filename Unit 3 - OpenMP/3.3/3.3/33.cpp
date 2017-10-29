// 3.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <omp.h>
#include <random>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>

//using namespace chrono;
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

//algorithm
void bubble_sort(vector<unsigned int> &values)
{
	for (size_t count = 1; count < values.size(); count++)
	{
		for (size_t i = 0; i < (values.size() - 1); i++)
		{
			if (values[i] > values[i + 1])
			{
				unsigned int temp = values[i];
				values[i] = values[i + 1];
				values[i + 1] = temp;
			}
		}
	}
}

int main()
{
	//Create results file
	ofstream results("bubble_sort.csv", ofstream::out);

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
			bubble_sort(data);
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

