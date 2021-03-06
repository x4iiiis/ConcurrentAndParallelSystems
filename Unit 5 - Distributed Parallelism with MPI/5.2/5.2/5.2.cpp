// 5.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <mpi.h>

using namespace std;

int main()
{
	//Initialise MPI
	auto result = MPI_Init(nullptr, nullptr);

	//Check that we initialised correctly
	if (result != MPI_SUCCESS)
	{
		//Display error and abort
		cout << "Error - initialising MPI!" << endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	//Display information
	cout << "Number of processors = " << num_procs << endl;
	cout << "My rank = " << rank << endl;
	cout << "Running on = " << host_name << endl;

	//Shutdown MPI
	MPI_Finalize();

    return 0;
}

