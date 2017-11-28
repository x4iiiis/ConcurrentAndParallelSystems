// 8.1.cpp : Defines the entry point for the console application.
//

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <vector>


using namespace std;

//8.2
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

//8.3
__global__ void vecadd(const int *A, const int *B, int *C)
{
	//Get block index
	unsigned int block_idx = blockIdx.x;
	//Get thread index
	unsigned int thread_idx = threadIdx.x;
	//Get the number of threads per block
	unsigned int block_dim = blockDim.x;
	//Get the thread's unique ID - (block_idx * block_dim) + thread_idx;
	unsigned int idx = (block_idx * block_dim) + thread_idx;
	//Add corresponding locations of A and B and store in C
	C[idx] = A[idx] + B[idx];
}

int main()
{
	//Initialise CUDA - select device
	cudaSetDevice(0);

	//8.2
	cuda_info();


	//From OpenCL
	const unsigned int ELEMENTS = 2048;

	//8.4
	//Create host memory
	auto data_size = sizeof(int) * ELEMENTS;
	vector<int> A(ELEMENTS);	//Input array
	vector<int> B(ELEMENTS);	//Input array
	vector<int> C(ELEMENTS);	//Output array
	//Initialise input data
	for (unsigned int i = 0; i < ELEMENTS; i++)
	{
		A[i] = B[i] = i;
	}
	//Declare buffers
	int *buffer_A, *buffer_B, *buffer_C;
	//Initialise buffers
	cudaMalloc((void**)&buffer_A, data_size);
	cudaMalloc((void**)&buffer_B, data_size);
	cudaMalloc((void**)&buffer_C, data_size);
	//Write host data to device
	cudaMemcpy(buffer_A, &A[0], data_size, cudaMemcpyHostToDevice);
	cudaMemcpy(buffer_B, &B[0], data_size, cudaMemcpyHostToDevice);

	
	//8.5
	//Run kernel with one thread for each element
	//First value is number of blocks, second is threads per block.Max 1024 threads per block
	vecadd<<<ELEMENTS / 1024, 1024 >>>(buffer_A, buffer_B, buffer_C);
	//Wait for kernel to complete
	cudaDeviceSynchronize();
	//Read output buffer back to the host
	cudaMemcpy(&C[0], buffer_C, data_size, cudaMemcpyDeviceToHost);

	//From OpenCL
	//Verify the output
	auto result = true;
	int i = 0;
	//Iterate through each value in result array
	for (auto &e : C)
	{
		//Check value
		if (e != i + i)
		{
			result = false;
			break;
		}
		i++;
	}
	//Check if result is true and display accordingly
	if (result)
	{
		cout << "Output is correct" << endl;
	}
	else
	{
		cout << "Output is incorrect" << endl;
	}

	//Clean up resources
	cudaFree(buffer_A);
	cudaFree(buffer_B);
	cudaFree(buffer_C);


	return 0;
}

