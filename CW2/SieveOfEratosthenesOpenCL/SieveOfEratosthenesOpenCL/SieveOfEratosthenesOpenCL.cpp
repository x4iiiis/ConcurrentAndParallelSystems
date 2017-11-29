// SieveOfEratosthenesOpenCL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <chrono>

#include "../../NVIDIA GPU Computing SDK/OpenCL/common/inc/CL/cl.h"
#include <vector>
#include <sstream>
#include <string>
#include <array>

using namespace std;
using namespace chrono;

//Initialising OpenCL
void initialise_opencl(vector<cl_platform_id> &platforms, vector<cl_device_id> &devices, cl_context &context, cl_command_queue &cmd_queue)
{
	//Status of OpenCL calls
	cl_int status;

	//Get the number of platforms
	cl_uint num_platforms;
	status = clGetPlatformIDs(0, nullptr, &num_platforms);

	//Resize vector to store platforms
	platforms.resize(num_platforms);

	//Fill in platform vector
	status = clGetPlatformIDs(num_platforms, &platforms[0], nullptr);

	//Assume platform 0 is the one we want to use
	//Get devices for platform 0
	cl_uint num_devices;
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);

	//Resize vector to store devices
	devices.resize(num_devices);

	//Fill in devices vector
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, &devices[0], nullptr);

	//Create a context
	context = clCreateContext(nullptr, num_devices, &devices[0], nullptr, nullptr, &status);

	//Create a command queue
	cmd_queue = clCreateCommandQueue(context, devices[0], 0, &status);
}


//7.3 Loads an OpenCL program
cl_program load_program(const string &filename, cl_context &context, cl_device_id &device, cl_int num_devices)
{
	//Status of OpenCL calls
	cl_int status;

	//Create and compile program
	//Read in kernel file
	ifstream input("kernel.cl", ifstream::in);
	stringstream buffer;
	buffer << input.rdbuf();

	//Get the character array of the file contents
	auto file_contents = buffer.str();
	auto char_contents = file_contents.c_str();//.str();

											   //Create program object
	auto program = clCreateProgramWithSource(context, 1, &char_contents, nullptr, &status);

	//Compile / build program
	status = clBuildProgram(program, num_devices, &device, nullptr, nullptr, nullptr);

	//Check if completed
	if (status != CL_SUCCESS)
	{
		//Error building - get log
		size_t length;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &length);

		char *log = new char[length];

		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, length, log, &length);

		//Print log
		cout << log << endl;

		delete[] log;
	}

	//Return program object
	return program;
}



int main()
{
	///////////////////////////////////////////////////////////////////////////////////////http://www.algolist.net/Algorithms/Number_theoretic/Sieve_of_Eratosthenes
	int Runs = 10;
	int upperBound = 1000000000;	//Supposed to be 1 billion (1000000000)




	//7.4
	//Number of elements and size of buffer on GPU
	const unsigned int elements = 1000000000;//2048;
	const unsigned int data_size = sizeof(bool) * elements;
	//Host data - stored in main memory
	array<bool*, false> A;
	array<bool*, false> B;
	//Initialise input data
	for (unsigned int i = 0; i < elements; i++)
	{
		A[i] = false;
	}




	//Status of OpenCL calls
	cl_int status;

	//Initialise OpenCL
	vector<cl_platform_id> platforms;
	vector<cl_device_id> devices;
	cl_context context;
	cl_command_queue cmd_queue;
	initialise_opencl(platforms, devices, context, cmd_queue);





	//Original sequential shit
	//R//Create time taken file
	ofstream timer("TimeTaken.csv", ofstream::out);					//Outside the loop so that it keeps track of previous runs 
	timer << "Milliseconds" << endl;

	for (int i = 0; i < Runs; i++)
	{
		cout << "Starting run " << i + 1 << " of " << Runs << endl;		//Will slightly affect performance but it allows to check the progress when running several times
																		//Won't affect time taken results though. 

		//R//Create results file
		ofstream results("PrimeNumbers.txt", ofstream::out);		//Inside the loop so that PrimeNumbers.txt is wiped each time and only keeps the prime numbers on the last run

		int upperBoundSquareRoot = (int)sqrt((double)upperBound);
		bool *isComposite = new bool[upperBound + 1, false];
		memset(isComposite, 0, sizeof(bool) * (upperBound + 1));


		//Pass *isComposite to the kernel?


		//R//Timing the results
		auto start = system_clock::now();

		//add this stuff to the kernel

		//for (int m = 2; m <= upperBoundSquareRoot; m++)
		//{
		//	if (!isComposite[m])
		//	{
		//		//cout << m << " ";			//Comment these out as they slow down performance
		//		results << m << endl;		//R//Outputting the prime numbers to the text file

		//		for (int k = m * m; k <= upperBound; k += m)
		//		{
		//			isComposite[k] = true;
		//		}
		//	}
		//}
		//for (int m = upperBoundSquareRoot; m <= upperBound; m++)
		//{
		//	if (!isComposite[m])
		//	{
		//		//cout << m << " ";			//Comment these out as they slow down performance
		//		results << m << endl;		//R//Outputting the prime numbers to the text file
		//	}
		//}

		//R//Timing the results
		auto end = system_clock::now();
		auto total = duration_cast<milliseconds>(end - start).count();

		//R//Outputting the time taken to a text file
		timer << total << endl;

		delete[] isComposite;
	}














	//Load program
	auto program = load_program("kernel.cl", context, devices[0], devices.size());

	//Create the kernel
	auto kernel = clCreateKernel(program, "EratosthenesOpenCL", &status);

	//Still 7.4 but dunno if it's meant to be here
	//Create device buffers - stored on GPU
	cl_mem buffer_A; //Input array on the device
	cl_mem buffer_B; //Input array on the device
	cl_mem buffer_C; //Output array on the device
	//Allocate buffer size
	buffer_A = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, nullptr, &status);
	buffer_B = clCreateBuffer(context, CL_MEM_WRITE_ONLY, data_size, nullptr, &status);

	//STILL 7.4 and still dunno where to put it
	status = clEnqueueWriteBuffer(cmd_queue, buffer_A, CL_FALSE, 0, data_size, A.data(), 0, nullptr, nullptr);
	//status = clEnqueueWriteBuffer(cmd_queue, buffer_B, CL_FALSE, 0, data_size, B.data(), 0, nullptr, nullptr);

	//More 7.4
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_A);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_B);
	//status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_C);

	//7.5 here?
	//Configure the work dimensions - 1D of elements
	array<size_t, 1> global_work_size = { elements };
	//Enqueue the kernel for execution
	status = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, nullptr, global_work_size.data(), nullptr, 0, nullptr, nullptr);
	//Read the output buffer from the GPU to main memory
	clEnqueueReadBuffer(cmd_queue, buffer_C, CL_TRUE, 0, data_size, B.data(), 0, nullptr, nullptr);



	//More 7.5
	//Verify the output
	//auto result = true;
	//int i = 0;
	////Iterate through each value in result array
	//for (auto &e : B)
	//{
	//	//Check value
	//	if (e != i + i)
	//	{
	//		result = false;
	//		break;
	//	}
	//	i++;
	//}
	////Check if result is true and display accordingly
	//if (result)
	//{
	//	cout << "Output is correct" << endl;
	//}
	//else
	//{
	//	cout << "Output is incorrect" << endl;
	//}




	//Free OpenCL resources
	clReleaseMemObject(buffer_A);
	clReleaseMemObject(buffer_B);
	//clReleaseMemObject(buffer_C);
	clReleaseCommandQueue(cmd_queue);
	clReleaseContext(context);
	clReleaseKernel(kernel);
	clReleaseProgram(program);

	return 0;
}

