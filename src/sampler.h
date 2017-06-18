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
	double uniform(double min = 0, double max = 0){
		uniform_real_distribution<double> rand(min, max);
		return rand(mt);
	}
} // namespace sampler