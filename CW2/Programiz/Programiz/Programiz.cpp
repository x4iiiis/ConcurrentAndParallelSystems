// Programiz.cpp : Defines the entry point for the console application.
//https://www.programiz.com/cpp-programming/examples/prime-number

#include "stdafx.h"
#include <iostream>

using namespace std;

int main()
{
	int n = 0;
	int i = 2;
	bool isPrime = true;

	//cout << "Enter a positive integer: ";
	//cin >> n;
	cout << "2 is a prime number trust me bruh" << endl;

	for (int R = 1; R < 1000; R++)
	{
		n = R;

		//for (i = 2; i <= n / 2; ++i)
		{
			if (n % i == 0)
			{
				isPrime = false;
				//break;
			}
		}
		if (isPrime)
			cout << R << " is a prime number" << endl;
		else
			cout << R << " is not a prime number" << endl;

		isPrime = true;
	}
	return 0;
}

