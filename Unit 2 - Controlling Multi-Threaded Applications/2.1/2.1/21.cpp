// 21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono;

void increment(shared_ptr<int> value)
{
	//Loop 1 million times, incrementing value
	for (unsigned int i = 0; i < 1000000; i++)
	{
		//Increment value
		*value = *value + 1;
	}
}

int main()
{
	//Create a shared int value
	auto value = make_shared<int>(0);

	//Create number of threads hardware natively supports
	auto num_threads = thread::hardware_concurrency();
	vector<thread> threads;
	for (unsigned int i = 0; i < num_threads; i++)
	{
		threads.push_back(thread(increment, value));
	}

	//Join the threads
	for (auto &t : threads)
	{
		t.join();
	}

	//Display the value
	cout << "Value = " << *value << endl;

    return 0;
}

