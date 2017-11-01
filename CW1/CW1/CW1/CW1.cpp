// CW1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
//R//
#include <thread>
#include <mutex>
#include <omp.h>



using namespace std;
using namespace std::chrono;

constexpr size_t MAX_DEPTH = 512; // Upper limit on recursion, increase this on systems with more stack size.
constexpr double PI = 3.14159265359;

template <class T, class Compare>
constexpr const T &clamp(const T &v, const T &lo, const T &hi, Compare comp)
{
	return assert(!comp(hi, lo)), comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template <class T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi)
{
	return clamp(v, lo, hi, std::less<>());
}

struct vec
{
	double x, y, z;

	vec(double x = 0, double y = 0, double z = 0) noexcept
		: x(x), y(y), z(z)
	{
	}

	vec operator+(const vec &other) const noexcept
	{
		return vec(x + other.x, y + other.y, z + other.z);
	}

	vec operator-(const vec &other) const noexcept
	{
		return vec(x - other.x, y - other.y, z - other.z);
	}

	vec operator*(double scale) const noexcept
	{
		return vec(x * scale, y * scale, z * scale);
	}

	vec mult(const vec &other) const noexcept
	{
		return vec(x * other.x, y * other.y, z * other.z);
	}

	vec normal() const noexcept
	{
		return *this * (1.0 / sqrt(x * x + y * y + z * z));
	}

	double dot(const vec &other) const noexcept
	{
		return x * other.x + y * other.y + z * other.z;
	}

	vec cross(const vec &other) const noexcept
	{
		return vec(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
};

struct ray
{
	vec origin, direction;

	ray(const vec &origin, const vec &direction) noexcept
		: origin(origin), direction(direction)
	{
	}
};

enum struct reflection_type { DIFFUSE, SPECULAR, REFRACTIVE };

struct sphere
{
	double radius;
	vec position;
	vec emission, colour;
	reflection_type reflection;

	sphere(double radius, const vec &position, const vec &emission, const vec &colour, reflection_type reflection) noexcept
		: radius(radius), position(position), emission(emission), colour(colour), reflection(reflection)
	{
	}

	double intersection(const ray &ray) const noexcept
	{
		static constexpr double eps = 1e-4;
		vec origin_position = position - ray.origin;
		double b = origin_position.dot(ray.direction);
		double determinant = b * b - origin_position.dot(origin_position) + radius * radius;
		if (determinant < 0)
		{
			return 0;
		}
		else
		{
			determinant = sqrt(determinant);
		}
		double t = b - determinant;
		if (t > eps)
		{
			return t;
		}
		else
		{
			t = b + determinant;
			if (t > eps)
			{
				return t;
			}
			else
			{
				return 0;
			}
		}
	}
};

inline bool intersect(const vector<sphere> &spheres, const ray &ray, double &distance, size_t &sphere_index) noexcept
{
	static constexpr double maximum_distance = 1e20;
	distance = maximum_distance;
	for (size_t index = 0; index < spheres.size(); ++index)
	{
		double temp_distance = spheres[index].intersection(ray);
		if (temp_distance > 0 && temp_distance < distance)
		{
			distance = temp_distance;
			sphere_index = index;
		}
	}
	return distance < maximum_distance;
}

vec radiance(const vector<sphere> &spheres, const ray &the_ray, int depth) noexcept
{
	static random_device rd;
	static default_random_engine generator(rd());
	static uniform_real_distribution<double> distribution;
	static auto get_random_number = bind(distribution, generator);

	double distance;
	size_t sphere_index;
	if (!intersect(spheres, the_ray, distance, sphere_index))
		return vec();
	const sphere &hit_sphere = spheres[sphere_index];
	vec hit_point = the_ray.origin + the_ray.direction * distance;
	vec intersection_normal = (hit_point - hit_sphere.position).normal();
	vec pos_intersection_normal = intersection_normal.dot(the_ray.direction) < 0 ? intersection_normal : intersection_normal * -1;
	vec colour = hit_sphere.colour;
	double max_reflection = max({ colour.x, colour.y, colour.z });
	if (depth > MAX_DEPTH)
	{
		return hit_sphere.emission;
	}
	else if (++depth > 5)
	{
		if (get_random_number() < max_reflection)
		{
			colour = colour * (1.0 / max_reflection);
		}
		else
		{
			return hit_sphere.emission;
		}
	}

	if (hit_sphere.reflection == reflection_type::DIFFUSE)
	{
		double r1 = 2.0 * PI * get_random_number();
		double r2 = get_random_number();
		vec w = pos_intersection_normal;
		vec u = ((abs(w.x) > 0.1 ? vec(0, 1, 0) : vec(1, 0, 0)).cross(w)).normal();
		vec v = w.cross(u);
		vec new_direction = (u * cos(r1) * sqrt(r2) + v * sin(r1) * sqrt(r2) + w * sqrt(1 - r2)).normal();
		return hit_sphere.emission + colour.mult(radiance(spheres, ray(hit_point, new_direction), depth));
	}
	else if (hit_sphere.reflection == reflection_type::SPECULAR)
	{
		return hit_sphere.emission + colour.mult(radiance(spheres, ray(hit_point, the_ray.direction - intersection_normal * 2 * intersection_normal.dot(the_ray.direction)), depth));
	}
	ray reflection_ray(hit_point, the_ray.direction - intersection_normal * 2 * intersection_normal.dot(the_ray.direction));
	bool into = intersection_normal.dot(pos_intersection_normal) > 0;
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc;
	double ddn = the_ray.direction.dot(pos_intersection_normal);
	double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
	if (cos2t < 0.0)
	{
		return hit_sphere.emission + colour.mult(radiance(spheres, reflection_ray, depth));
	}
	vec tdir = (the_ray.direction * nnt - intersection_normal * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).normal();
	double a = nt - nc;
	double b = nt + nc;
	double R0 = a * a / (b * b);
	double c = 1 - (into ? -ddn : tdir.dot(intersection_normal));
	double Re = R0 + (1 - R0) * c * c * c * c * c;
	double Tr = 1 - Re;
	double P = 0.25 + 0.5 * Re;
	double RP = Re / P;
	double TP = Tr / (1.0 - P);
	if (depth > 2)
	{
		if (get_random_number() < P)
		{
			return hit_sphere.emission + colour.mult(radiance(spheres, reflection_ray, depth) * RP);
		}
		else
		{
			return hit_sphere.emission + colour.mult(radiance(spheres, ray(hit_point, tdir), depth) * TP);
		}
	}
	else
	{
		return hit_sphere.emission + colour.mult(radiance(spheres, reflection_ray, depth) * Re + radiance(spheres, ray(hit_point, tdir), depth) * Tr);
	}
}

struct lwrite
{
	unsigned long value;
	unsigned size;

	lwrite(unsigned long value, unsigned size) noexcept
		: value(value), size(size)
	{
	}
};

inline std::ostream &operator<<(std::ostream &outs, const lwrite &v)
{
	unsigned long value = v.value;
	for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
		outs.put(static_cast<char>(value & 0xFF));
	return outs;
}

bool array2bmp(const std::string &filename, const vector<vec> &pixels, const size_t width, const size_t height)
{
	std::ofstream f(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (!f)
	{
		return false;
	}
	// Write Bmp file headers
	const size_t headers_size = 14 + 40;
	const size_t padding_size = (4 - ((height * 3) % 4)) % 4;
	const size_t pixel_data_size = width * ((height * 3) + padding_size);
	f.put('B').put('M'); // bfType
						 // bfSize
	f << lwrite(headers_size + pixel_data_size, 4);
	// bfReserved1, bfReserved2
	f << lwrite(0, 2) << lwrite(0, 2);
	// bfOffBits, biSize
	f << lwrite(headers_size, 4) << lwrite(40, 4);
	// biWidth,  biHeight,  biPlanes
	f << lwrite(width, 4) << lwrite(height, 4) << lwrite(1, 2);
	// biBitCount, biCompression = BI_RGB ,biSizeImage
	f << lwrite(24, 2) << lwrite(0, 4) << lwrite(pixel_data_size, 4);
	// biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant
	f << lwrite(0, 4) << lwrite(0, 4) << lwrite(0, 4) << lwrite(0, 4);
	// Write image data
	for (size_t x = height; x > 0; x--)
	{
		for (size_t y = 0; y < width; y++)
		{
			const auto &val = pixels[((x - 1) * width) + y];
			f.put(static_cast<char>(int(255.0 * val.z))).put(static_cast<char>(int(255.0 * val.y))).put(static_cast<char>(255.0 * val.x));
		}
		if (padding_size)
		{
			f << lwrite(0, padding_size);
		}
	}
	return f.good();
}

//R//
void pixelsI(int dimension, int samples, vec r, vec cx, vec cy, ray camera, vector<sphere> &spheres, vector<vec> &pixels, int threadStart, int threadEnd)
{
	random_device rd;
	default_random_engine generator(rd());
	uniform_real_distribution<double> distribution;
	auto get_random_number = bind(distribution, generator);

	//R//
	mutex muteX;

	//for (size_t y = 0; y < dimension; ++y)
	for (int y = threadStart; y < threadEnd; ++y)
	{
		cout << "Rendering " << dimension << " * " << dimension << "pixels. Samples:" << samples * 4 << " spp (" << 100.0 * y / (dimension - 1) << ")" << endl;
		for (size_t x = 0; x < dimension; ++x)
		{
			for (size_t sy = 0, i = (dimension - y - 1) * dimension + x; sy < 2; ++sy)
			{
				for (size_t sx = 0; sx < 2; ++sx)
				{
					r = vec();
					for (size_t s = 0; s < samples; ++s)
					{
						double r1 = 2 * get_random_number(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * get_random_number(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						vec direction = cx * static_cast<double>(((sx + 0.5 + dx) / 2 + x) / dimension - 0.5) + cy * static_cast<double>(((sy + 0.5 + dy) / 2 + y) / dimension - 0.5) + camera.direction;
						r = r + radiance(spheres, ray(camera.origin + direction * 140, direction.normal()), 0) * (1.0 / samples);
					}
					//R//Lock Mutex
					muteX.lock();
					pixels[i] = pixels[i] + vec(clamp(r.x, 0.0, 1.0), clamp(r.y, 0.0, 1.0), clamp(r.z, 0.0, 1.0)) * 0.25;
					muteX.unlock();
				}
			}
		}
	}
}


//Main for Mutex
int main(int argc, char **argv)
{
	// *** These parameters can be manipulated in the algorithm to modify work undertaken ***
	constexpr size_t dimension = 400;
	vector<sphere> spheres
	{
		sphere(1e5, vec(1e5 + 1, 40.8, 81.6), vec(), vec(0.75, 0.25, 0.25), reflection_type::DIFFUSE),
		sphere(1e5, vec(-1e5 + 99, 40.8, 81.6), vec(), vec(0.25, 0.25, 0.75), reflection_type::DIFFUSE),
		sphere(1e5, vec(50, 40.8, 1e5), vec(), vec(0.75, 0.75, 0.75), reflection_type::DIFFUSE),
		sphere(1e5, vec(50, 40.8, -1e5 + 170), vec(), vec(), reflection_type::DIFFUSE),
		sphere(1e5, vec(50, 1e5, 81.6), vec(), vec(0.75, 0.75, 0.75), reflection_type::DIFFUSE),
		sphere(1e5, vec(50, -1e5 + 81.6, 81.6), vec(), vec(0.75, 0.75, 0.75), reflection_type::DIFFUSE),
		sphere(16.5, vec(27, 16.5, 47), vec(), vec(1, 1, 1) * 0.999, reflection_type::SPECULAR),
		sphere(16.5, vec(73, 16.5, 78), vec(), vec(1, 1, 1) * 0.999, reflection_type::REFRACTIVE),
		sphere(600, vec(50, 681.6 - 0.27, 81.6), vec(12, 12, 12), vec(), reflection_type::DIFFUSE)
	};
	// **************************************************************************************

	ray camera(vec(50, 52, 295.6), vec(0, -0.042612, -1).normal());
	vec cx = vec(0.5135);
	vec cy = (cx.cross(camera.direction)).normal() * 0.5135;
	vec r;
	vector<vec> pixels(dimension * dimension);


	//R//Create results file
	ofstream results("test.csv", ofstream::out);
	results << "Image Dimensions (px)" << "," << "Samples per Pixel" << "," << "Time taken (ms)" << endl;

	//constexpr 
	size_t samples = 4; // Algorithm performs 4 * samples per pixel.

	//R//
	//Trying to make a loop
	for (int R = 0; R < 10; R++)
	{


	//R//
	auto threadCount = thread::hardware_concurrency();
	vector<thread> threads;

	//R//(C)
	for (int i = 0; i < threadCount; i++)
	{
		//C//
		int threadChunk = dimension / threadCount;
		int threadStart = i * threadChunk;
		int threadEnd = (i + 1) * threadChunk;
		//C^^^^//

		threads.push_back(thread(pixelsI, dimension, samples, r, cx, cy, camera, spheres, std::ref(pixels), threadStart, threadEnd));
	}

	//R//Get Start Time
	auto startTime = chrono::system_clock::now();

	//R//Join Threads
	for (auto &t : threads)
	{
		t.join();
	}

	//R//Get end time and calculate the time taken
	auto endTime = chrono::system_clock::now();
	auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

	//R//Output results
	results << dimension << " * " << dimension << ", " << samples * 4 << "," << totalTime << endl;

	//R//Clear the threads for the loop
	threads.clear();

	//R//End of loop
	}

	//Create the image file
	cout << "img.bmp" << (array2bmp("img.bmp", pixels, dimension, dimension) ? " Saved\n" : " Save Failed\n");

	return 0;
}

////KEVIN'S MAIN//
//int main(int argc, char **argv)
//{
//	random_device rd;
//	default_random_engine generator(rd());
//	uniform_real_distribution<double> distribution;
//	auto get_random_number = bind(distribution, generator);
//
//	// *** These parameters can be manipulated in the algorithm to modify work undertaken ***
//	constexpr size_t dimension = 400;
//	constexpr size_t samples = 4; // Algorithm performs 4 * samples per pixel.
//	vector<sphere> spheres
//	{
//		sphere(1e5, vec(1e5 + 1, 40.8, 81.6), vec(), vec(0.75, 0.25, 0.25), reflection_type::DIFFUSE),
//		sphere(1e5, vec(-1e5 + 99, 40.8, 81.6), vec(), vec(0.25, 0.25, 0.75), reflection_type::DIFFUSE),
//		sphere(1e5, vec(50, 40.8, 1e5), vec(), vec(0.75, 0.75, 0.75), reflection_type::DIFFUSE),
//		sphere(1e5, vec(50, 40.8, -1e5 + 170), vec(), vec(), reflection_type::DIFFUSE),
//		sphere(1e5, vec(50, 1e5, 81.6), vec(), vec(0.75, 0.75, 0.75), reflection_type::DIFFUSE),
//		sphere(1e5, vec(50, -1e5 + 81.6, 81.6), vec(), vec(0.75, 0.75, 0.75), reflection_type::DIFFUSE),
//		sphere(16.5, vec(27, 16.5, 47), vec(), vec(1, 1, 1) * 0.999, reflection_type::SPECULAR),
//		sphere(16.5, vec(73, 16.5, 78), vec(), vec(1, 1, 1) * 0.999, reflection_type::REFRACTIVE),
//		sphere(600, vec(50, 681.6 - 0.27, 81.6), vec(12, 12, 12), vec(), reflection_type::DIFFUSE)
//	};
//	// **************************************************************************************
//
//	ray camera(vec(50, 52, 295.6), vec(0, -0.042612, -1).normal());
//	vec cx = vec(0.5135);
//	vec cy = (cx.cross(camera.direction)).normal() * 0.5135;
//	vec r;
//	vector<vec> pixels(dimension * dimension);
//

	//R//Create results file
	//ofstream results("test.csv", ofstream::out);
	//results << "Image Dimensions (px)" << "," << "Samples per Pixel" << "," << "Time taken (ms)" << endl;


	//R//
	//auto startTime = chrono::system_clock::now();

//	for (size_t y = 0; y < dimension; ++y)
//	{
//		cout << "Rendering " << dimension << " * " << dimension << "pixels. Samples:" << samples * 4 << " spp (" << 100.0 * y / (dimension - 1) << ")" << endl;
//		for (size_t x = 0; x < dimension; ++x)
//		{
//			for (size_t sy = 0, i = (dimension - y - 1) * dimension + x; sy < 2; ++sy)
//			{
//				for (size_t sx = 0; sx < 2; ++sx)
//				{
//					r = vec();
//					for (size_t s = 0; s < samples; ++s)
//					{
//						double r1 = 2 * get_random_number(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
//						double r2 = 2 * get_random_number(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
//						vec direction = cx * static_cast<double>(((sx + 0.5 + dx) / 2 + x) / dimension - 0.5) + cy * static_cast<double>(((sy + 0.5 + dy) / 2 + y) / dimension - 0.5) + camera.direction;
//						r = r + radiance(spheres, ray(camera.origin + direction * 140, direction.normal()), 0) * (1.0 / samples);
//					}
//					pixels[i] = pixels[i] + vec(clamp(r.x, 0.0, 1.0), clamp(r.y, 0.0, 1.0), clamp(r.z, 0.0, 1.0)) * 0.25;
//				}
//			}
//		}
//	}
	//R//Get end time and calculate the time taken
	//auto endTime = chrono::system_clock::now();
	//auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

	//R//Output results
	///results << dimension << " * " << dimension << ", " << samples * 4 << "," << totalTime << endl;

//	cout << "img.bmp" << (array2bmp("img.bmp", pixels, dimension, dimension) ? " Saved\n" : " Save Failed\n");
//	return 0;
//}
//
