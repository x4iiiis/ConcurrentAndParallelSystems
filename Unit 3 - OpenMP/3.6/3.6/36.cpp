// 36.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

//Let's create a function that relies on i to determine the amound of work
double f(unsigned int i)
{
	//Calculate start and end values
	auto start = i * (i + 1) / 2;
	auto end = start + i;

	//Declare return value
	auto result = 0.0;

	//Loop for number of iterations, calculating sin
	for (auto j = start; j <= end; j++)
	{
		result += sin(j);
	}

	//Return result
	return result;
}

int main()
{
	//Get number of hardware threads
	auto thread_count = thread::hardware_concurrency();

	//Define number of iterations to calculate
	int n = static_cast<int>(pow(2, 14));

	//Declare sum value
	auto sum = 0.0;

	//get start time
	auto start = chrono::system_clock::now();

#pragma omp parallel for num_threads(thread_count) reduction(+:sum) schedule(static, 1)

	for (auto i = 0; i <= n; i++)
	{
		sum += f(i);
	}

	//Get end time
	auto end = chrono::system_clock::now();

	//Calculate and output total time
	auto total = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	std::cout << "Total time: " << total << "ms" << endl;

    return 0;
}

