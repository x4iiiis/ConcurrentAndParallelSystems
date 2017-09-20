// 15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>
#include <iostream>

using namespace std;

int main()
{
	//Create a thread using a lambda expression
	thread t([] {cout << "Hello from lambda thread!" << endl; });

	//Join thread
	t.join();

    return 0;
}

