// 4.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <chrono>
#include <random>
#include <xmmintrin.h>
#include <iostream>

using namespace std;
using namespace chrono;

//Randomly generate vector values
void generate_data(float *data, unsigned int num_values)
{
	//Create random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	default_random_engine e(static_cast<unsigned int>(millis.count()));

	//Fill data
	for (unsigned int i = 0; i < num_values; i++)
	{
		data[i] = e();
	}
}

//Normalises the vector
void normalise_vectors(__m128 *data, __m128 *result, unsigned int num_vectors)
{
	//Normalise the vectors
	for (unsigned int i = 0; i < num_vectors; i++)
	{
		//Square each component - simply multiply the vectors by themselves
		result[i] = _mm_mul_ps(data[i], data[i]);

		//Calculate sum of the components - store in all
		result[i].m128_f32[0] = result[i].m128_f32[1] = result[i].m128_f32[2] = result[i].m128_f32[3] =
			result[i].m128_f32[0] + result[i].m128_f32[1] + result[i].m128_f32[2] + result[i].m128_f32[3];

		//Calculate reciprocal square root of the values
		//that is 1.0f . sqrt(value) or the length of the vector
		result[i] = _mm_rsqrt_ps(result[i]);

		//Multiply result by the original data
		//As we have the recipricol it is the same as dividing each component by the length
		result[i] = _mm_mul_ps(data[i], result[i]);
	}
	//All vectors now normalised
}


//Check the first 100 results
void check_results(__m128 *data, __m128 *result)
{
	//Convert to floats
	auto float_data = (float*)data;
	auto float_res = (float*)result;

	//Check first 100 values
	for (unsigned int i = 0; i < 100; i++)
	{
		//Calculate the length of the vector
		float l = 0.0f;

		//Square each component and add to l
		for (unsigned int j = 0; j < 4; j++)
		{
			l += powf(float_data[(i * 4) + j], 2.0f);
		}

		//Square the length
		l = sqrtf(l);

		//Now check that the individual results
		for (unsigned int j = 0; j < 4; j++)
		{
			cout << float_data[(i * 4) + j] / l << " : " << float_res[(i * 4) + j] << endl;
		}
	}
}

int main()
{
	//Algorithm 3... 


    return 0;
}

