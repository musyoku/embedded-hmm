#pragma once
#include <vector>
#include <cmath>
#include "sampler.h"

class HMM{
private:
	vector<double*> _pool;
public:
	int _pool_capacity;
	double _sigma;
	double _tau;
	double _eta;
	HMM(int pool_capacity, double sigma, double tau, double eta){
		_pool_capacity = 0;
	}
	void init_pool(int seq_length){
		for(int t = 0;t < seq_length;t++){
			double* pool = new double[_pool_capacity];
			for(int k = 0;k < _pool_capacity;k++){
				pool[k] = sampler::normal(0, 1);
			}
			_pool.push_back(pool);
		}	
	}
	double compute_likelihood(double x, double mean, double std){
		return exp(-(x - mean) * (x - mean) / (2.0 * std * std)) / sqrt(2.0 * M_PI * std * std);
	}
	double compute_observation_probability(double y, double x){
		return compute_likelihood(y, x, _sigma);
	}
	double compute_transition_probability(double new_x, double old_x){
		return compute_likelihood(new_x, tanh(_eta * old_x), _tau);
	}
	double compute_pool_probability(double x){
		return compute_likelihood(x, 0.0, 1.0);
	}
	void perform_forward_backward(vector<double> &output_sequence, vector<double> &sampled_state_sequence){
		init_pool(output_sequence.size());
		vector<double*> u;
		vector<double*> w;
		int T = output_sequence.size();
		for(int t = 0;t < T;t++){
			double y = output_sequence[t];
			double* x = _pool[t];
			double* ut = new double[_pool_capacity];
			double z = 0;
			for(int j = 0;j < _pool_capacity;j++){
				ut[j] = compute_observation_probability(y, x[j]) / compute_pool_probability(x[j]);
				z += ut[j];
			}
			u.push_back(ut);
		}













	}
};