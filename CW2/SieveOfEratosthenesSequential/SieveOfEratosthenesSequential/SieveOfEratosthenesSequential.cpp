// SieveOfEratosthenesSequential.cpp : Defines the entry point for the console application.
//

/*
In mathematics, the sieve of Eratosthenes is a simple, ancient algorithm for finding all prime numbers up to any given limit.

It does so by iteratively marking as composite (i.e., not prime) the multiples of each prime, starting with the first prime number, 2. 

The multiples of a given prime are generated as a sequence of numbers starting from that prime, with constant difference between them that is equal to that prime.[1] 
This is the sieve's key distinction from using trial division to sequentially test each candidate number for divisibility by each prime.[2]

The earliest known reference to the sieve (Ancient Greek: κόσκινον Ἐρατοσθένους, kóskinon Eratosthénous) is in Nicomachus of Gerasa's Introduction to Arithmetic,[3] 
which describes it and attributes it to Eratosthenes of Cyrene, a Greek mathematician.

One of a number of prime number sieves, it is one of the most efficient ways to find all of the smaller primes. It may be used to find primes in arithmetic progressions.[4]
*/

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <math.h>

using namespace std;


struct Number
{
	unsigned int number = 0;
	bool prime = true;
};


int main()
{

	/*
	
	//For initial testing purposes we'll use 100 as our n before we bump it up to 1000000000
	unsigned int n = 100; //Should this be a long or something for 1bn?

	vector<Number> nums;

	for (unsigned int i = 2; i < n; i++)
	{
		Number newnum;
		newnum.number = i;
		nums.push_back(newnum);
	}


	for (unsigned int i = 2; i < sqrt(n); i++)
	{
		if (nums[i].prime == true)
		{
			unsigned int j = 0;

			for (unsigned int iterator = 0; j < n; iterator++)
			{
				j = (i*i)+(iterator*i);
				nums[j].prime = false;
			}
		}
	}



	for (auto &n : nums)
	{
		if (n.prime == true) 
		{
			cout << n.number << endl;
		}
	}

	*/

	
	
	
	//http://www.sanfoundry.com/cpp-program-implement-sieve-eratosthenes/
	const int n = 1000;

	vector<int> nums;
	
	for (unsigned int i = 0; i < n; i++)
	{
		nums.push_back(i);
	}
	//int arr[n] = { 0 };

	for (int i = 2; i < n; i++)
	{
		for (int j = i * i; j < n; j += i)
		{
			nums[j - 1] = 1;
		}
	}
	for (int i = 1; i < n; i++)
	{
		if (nums[i - 1] == 0)
		{
			cout << i << endl;
		}
	}
	




    return 0;
}

/*
Pseudocode
The sieve of Eratosthenes can be expressed in pseudocode, as follows:

Input: an integer n > 1.

Let A be an array of Boolean values, indexed by integers 2 to n,
initially all set to true.

for i = 2, 3, 4, ..., not exceeding √n:
if A[i] is true:
for j = i2, i2+i, i2+2i, i2+3i, ..., not exceeding n:
A[j] := false.

Output: all i such that A[i] is true.
*/