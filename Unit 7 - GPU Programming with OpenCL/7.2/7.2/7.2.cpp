// 7.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CL/cl.h"
#include <vector>
#include <iostream>

using namespace std;


//Helper method to print OpenCL device info
void print_opencl_info(vector<cl_device_id> &devices)
{
	//Buffers for device name and vendor
	char device_name[1024], vendor[1024];

	//Declare other necessary variables
	cl_uint num_cores;
	cl_long memory;
	cl_uint clock_freq;
	cl_bool available;

	//Iterate through each device in vector and display information
	for (auto &d : devices)
	{
		//Get info for device
		clGetDeviceInfo(d, CL_DEVICE_NAME, 1024, device_name, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_VENDOR, 2014, vendor, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_cores, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_long), &memory, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &clock_freq, nullptr);
		clGetDeviceInfo(d, CL_DEVICE_AVAILABLE, sizeof(cl_bool), &available, nullptr);

		//Print info
		cout << "Device: " << device_name << endl;
		cout << "Vendor: " << vendor << endl;
		cout << "Cores: " << num_cores << endl;
		cout << "Memory: " << memory / (1024 * 1024) << "MB" << endl;
		cout << "Clock freq: " << clock_freq << "MHz" << endl;
		cout << "Available: " << available << endl;
		cout << "***********************" << endl; << endl;
	}
}


int main()
{
    return 0;
}

