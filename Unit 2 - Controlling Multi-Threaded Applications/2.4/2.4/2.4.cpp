// 2.4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <condition_variable>
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
using namespace std::this_thread;

mutex mut;

void task_1(condition_variable &condition)
{
	//Task one will initially sleep for a few seconds
	cout << "Task 1 sleeping for 3 seconds" << endl;
	sleep_for(seconds(3));

	//Notify waiting thread
	cout << "Task 1 notifying waiting thread" << endl;
	condition.notify_one();

	//Now wait for notification
	cout << "Task 1 waiting for notification" << endl;
	condition.wait(unique_lock<mutex>(mut));

	//we are free to continue
	cout << "Task 1 notified" << endl;

	//Sleep for 3 seconds
	cout << "Task 1 sleeping for 3 seconds" << endl;
	sleep_for(seconds(3));

	//Notify any waiting thread
	cout << "Task 1 notifying waiting thread" << endl;
	condition.notify_one();

	//Now wait 3 seconds for notification
	cout << "Task 1 waiting 3 seconds for notification" << endl;
	if (condition.wait_for(unique_lock<mutex>(mut), seconds(3)) == cv_status::timeout)
	{
		cout << "Task 1 notified before 3 seconds" << endl;
	}
	else
	{
		cout << "Task 1 got tired waiting" << endl;
	}

	//Print finished message
	cout << "Task 1 finished" << endl;
}

void task_2(condition_variable &condition)
{
	//Task two will initially wait for notification
	cout << "Task 2 waiting for notification" << endl;
	condition.wait(unique_lock<mutex>(mut));

	//We are free to continue
	cout << "Task 2 notified" << endl;

	//Sleep for 5 seconds
	cout << "Task 2 sleeping for 5 seconds" << endl;
	sleep_for(seconds(5));

	//Notify waiting thread
	cout << "Task 2 notifying waiting thread" << endl;
	condition.notify_one();

	//Now wait 5 seconds for notification
	cout << "Task 2 waiting 5 seconds for notification" << endl;
	if (condition.wait_for(unique_lock<mutex>(mut), seconds(5)) == cv_status::timeout)
	{
		cout << "Task 2 notified before 5 seconds" << endl;
	}
	else
	{
		cout << "Task 2 got tired waiting" << endl;
	}

	//Sleep for 5 seconds
	cout << "Task 2 sleeping for 5 seconds" << endl;
	sleep_for(seconds(5));

	//Notify any waiting thread
	cout << "Task 2 notifying waiting thread" << endl;
	condition.notify_one();

	//Print finished message
	cout << "Task 2 finished" << endl;
}

int main()
{
	//Create condition variable
	condition_variable condition;

	//create two threads
	thread t1(task_1, ref(condition));
	thread t2(task_2, ref(condition));

	//Join two threads
	t1.join();
	t2.join();

	return 0;
}

