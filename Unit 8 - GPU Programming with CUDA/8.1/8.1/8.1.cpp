// 8.1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>


int main()
{
	//Initialise CUDA - select device
	cudaSetDevice(0);

    return 0;
}

