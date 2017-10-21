// 2.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono;

mutex mut;

void increment(shared_ptr<int> value)
{
	//Loop 1 million times, incrementing value
	for (unsigned int i = 0; i < 1000000; i++)
	{
		//Create the lock guard - automatically acquires mutex
		lock_guard<mutex> lock(mut);

		//Increment value
		*value = *value + 1;

		//Lock guard is automatically destroyed at the end of the loop scope - this will release the lock
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