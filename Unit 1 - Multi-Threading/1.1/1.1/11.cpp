// 11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <thread>
#include <iostream>

using namespace std;

//This is the function called by the thread
void hello_world()
{
	cout << "Hello from thread " << this_thread::get_id() << endl;
}

int main()
{
	//Create a new thread
	thread t(hello_world);
	
	//wait for thread to finish (join it)
	t.join();

	//return 0 (OK)
    return 0;
}

