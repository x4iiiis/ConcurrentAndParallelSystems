
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

void cuda_info()
{
	//Get CUDA device
	int device;
	cudaGetDevice(&device);

	//Get CUDA device properties
	cudaDeviceProp properties;
	cudaGetDeviceProperties(&properties, device);

	//Display properties
	cout << "Name: " << properties.name << endl;
	cout << "CUDA Capability: " << properties.major << "." << properties.minor << endl;
	cout << "Cores: " << properties.multiProcessorCount << endl;
	cout << "Memory: " << properties.totalGlobalMem / (1024 * 1024) << "MB" << endl;
	cout << "Clock Freq: " << properties.clockRate / 1000 << "MHz" << endl;
}


__global__ void Eratosthenes(bool *A)// , bool *B)
{
	//Get block index
	unsigned int block_idx = blockIdx.x;
	//Get thread index
	unsigned int thread_idx = threadIdx.x;
	//Get the number of threads per block
	unsigned int block_dim = blockDim.x;
	//Get the thread's unique ID - (block_idx * block_dim) + thread_idx;
	unsigned int idx = (block_idx * block_dim) + thread_idx;


	int upperBound = 1000000000;
	int upperBoundSquareRoot = (int)sqrt((double)upperBound);
	for (int m = 2; m <= upperBoundSquareRoot; m++)
	{
		if (!A[m])
		{
			cout << m << " ";			//Comment these out as they slow down performance
			//results << m << endl;		//R//Outputting the prime numbers to the text file

			for (int k = m * m; k <= upperBound; k += m)
			{
				A[k] = true;
			}
		}
	}
	for (int m = upperBoundSquareRoot; m <= upperBound; m++)
	{
		if (!A[m])
		{
			cout << m << " ";			//Comment these out as they slow down performance
			//results << m << endl;		//R//Outputting the prime numbers to the text file
		}
	}


	
	
	
	
	//Add corresponding locations of A and B and store in C
	//B[idx] = A[idx];	//Think this is how we "return" the result
}

int main()
{
	//Initialise CUDA - select device
	cudaSetDevice(0);

	//8.2
	cuda_info();


	//SieveOfEratosthenes
	const unsigned int ELEMENTS = 1000000000;
	//Change upperBound to ELEMENTS at some point
	const unsigned int upperBound = 1000000000;

	int upperBoundSquareRoot = (int)sqrt((double)upperBound);
	bool *isComposite = new bool[upperBound + 1];
	memset(isComposite, 0, sizeof(bool) * (upperBound + 1));









	//Create host memory
	auto data_size = sizeof(bool) * ELEMENTS;
	vector<bool> A(ELEMENTS);	//Input array
	//vector<bool> B(ELEMENTS);	//Output array

	//Initialise input data
	for (unsigned int i = 0; i < ELEMENTS; i++)
	{
		A[i] = false; 
	}

	//Declare buffers
	bool *buffer_A;//, *buffer_B;

	//Initialise Buffers
	cudaMalloc((void**)&buffer_A, data_size);
	//cudaMalloc((void**)&buffer_B, data_size);

	//Write host data to device
	cudaMemcpy(buffer_A, &A[0], data_size, cudaMemcpyHostToDevice);

	//Run kernel with one thread for each element
	//First value is number of blocks, second is threads per block.Max 1024 threads per block
	Eratosthenes<<<ELEMENTS / 1024, 1024>>>(buffer_A);// , buffer_B);
	//Wait for kernel to complete
	cudaDeviceSynchronize();
	//Read output buffer back to the host
	//cudaMemcpy(&B[0], buffer_B, data_size, cudaMemcpyDeviceToHost);



	//Clean up resources
	cudaFree(buffer_A);
	//cudaFree(buffer_B);
	
	return 0;
}