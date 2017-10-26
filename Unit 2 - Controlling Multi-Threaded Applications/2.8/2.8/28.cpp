// 28.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>
#include <chrono>
#include <memory>
#include <iostream>
#include <atomic>
#include <vector>

using namespace std;


void task(unsigned int id, shared_ptr<atomic_flag> flag)
{
	//Do 10 iterations
	for (unsigned int i = 0; i < 10; i++)
	{
		//Test the flag is available, and grab it when it is
		//Notice this while loops keeps spinning until flag is clear
		while (flag->test_and_set());
		//Flag is available. Thread displays message
		cout << "Thread " << id << " running " << i << endl;
		//Sleep for 1 second
		this_thread::sleep_for(chrono::seconds(1));
		//Clear the flag
		flag->clear();
	}
}

int main()
{
	//Create shared flag
	auto flag = make_shared<atomic_flag>();

	//Get number of hardware threads
	auto num_threads = thread::hardware_concurrency();

	//Create threads
	vector<thread> threads;
	for (unsigned int i = 0; i < num_threads; i++)
	{
		threads.push_back(thread(task, i, flag));
	}

	//Join threads
	for (auto &t : threads)
	{
		t.join();
	}

    return 0;
}

