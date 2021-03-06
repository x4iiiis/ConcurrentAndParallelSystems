// 4.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <malloc.h>
#include <xmmintrin.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

//Size of data to allocate - divide by four to get number of vectors
const unsigned int SIZE = static_cast<unsigned int>(pow(2, 24));
const unsigned int NUM_VECTORS = SIZE / 4;

int main()
{
	//Data - aligned to 16 bytes (128-bits)
	auto data = (float*)_aligned_malloc(SIZE * sizeof(float), 16);

	//Initialise data
	for (unsigned int i = 0; i < SIZE; i++)
	{
		//Set all values to 1
		data[i] = 1.0f;
	}

	//Value to add to all values
	auto value = _mm_set1_ps(4.0f);

	//__128m pointer to the data
	auto stream_data = (__m128*)data;

	//Start timer - use high resolution clock
	auto start = high_resolution_clock::now();

	//Add value to stream data
	for (unsigned int i = 0; i < NUM_VECTORS; i++)
	{
		stream_data[i] = _mm_add_ps(stream_data[i], value);
	}

	//End timer
	auto end = high_resolution_clock::now();

	//Calculate time and display
	auto total = duration_cast<microseconds>(end - start).count();
	cout << "SIMD: " << total << "micros" << endl;

	//Free memory
	_aligned_free(data);

	//Declare standard data
	data = new float[SIZE];

	//Set all values to 1
	for (unsigned int i = 0; i < SIZE; i++)
	{
		data[i] = 1.0f;
	}

	//Start timer
	start = high_resolution_clock::now();

	for (unsigned int i = 0; i < SIZE; i++)
	{
		data[i] = data[i] + 4.0f;
	}

	//End timer
	end = high_resolution_clock::now();

	//Calculate time and display
	total = duration_cast<microseconds>(end - start).count();
	cout << "Non-SIMD: " << total << "micros" << endl;

	//Free memory
	delete[] data;

    return 0;
}

