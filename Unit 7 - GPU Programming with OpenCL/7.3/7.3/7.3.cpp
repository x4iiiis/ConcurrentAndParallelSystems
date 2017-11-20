// 7.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CL/cl.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

//Loads an OpenCL program
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
	auto char_contents = file_contents.str();

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
	//Status of OpenCL calls
	cl_int status;

	//Initialise OpenCL
	vector<cl_platform_id> platforms;
	vector<cl_device_id> devices;
	cl_context context;
	cl_command_queue cmd_queue;
	initialise_opencl(platforms, devices, context, cmd_queue);

	//Print info
	print_opencl_info(devices);

	//Load program
	auto program = load_program("kernel.cl", context, devices[0], devices.size());

	//Create the kernel
	auto kernel = clCreateKernel(program, "vecadd", &status);

	//Free OpenCL resources
	clReleaseCommandQueue(cmd_queue);
	clReleaseContext(context);
	clReleaseKernel(kernel);
	clReleaseProgram(program);

    return 0;
}

