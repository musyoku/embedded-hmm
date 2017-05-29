#pragma once
#include <chrono>
#include <random>
using namespace std;

namespace sampler{
	int seed = chrono::system_clock::now().time_since_epoch().count();
	// int seed = 1;
	mt19937 mt(seed);
	double normal(double mean, double stddev){
		normal_distribution<double> rand(mean, stddev);
		return rand(mt);
	}
} // namespace sampler