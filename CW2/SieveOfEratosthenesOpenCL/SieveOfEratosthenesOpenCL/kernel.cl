__kernel void EratosthenesOpenCL(__global bool* isComposite)
{
	//Get the work item's unique ID
	int idx = get_global_id(0);



	for (int m = 2; m <= upperBoundSquareRoot; m++)
	{
		if (!isComposite[m])
		{
			//cout << m << " ";			//Comment these out as they slow down performance
			results << m << endl;		//R//Outputting the prime numbers to the text file				//store this shit somewhere rather than outputting it from the kernel?

			for (int k = m * m; k <= upperBound; k += m)
			{
				isComposite[k] = true;
			}
		}
	}
	for (int m = upperBoundSquareRoot; m <= upperBound; m++)
	{
		if (!isComposite[m])
		{
			//cout << m << " ";			//Comment these out as they slow down performance
			results << m << endl;		//R//Outputting the prime numbers to the text file				//store this shit somewhere rather than outputting it from the kernel?
		}
	}

	//Shit from vecadd
	C[idx] = A[idx] + B[idx];

}