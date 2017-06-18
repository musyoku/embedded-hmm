#pragma once
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include "sampler.h"
using namespace std;

class HMM{
private:
	vector<double*> _pool;
	double* _sampling_table;
public:
	int _pool_capacity;
	double _sigma;
	double _tau;
	double _eta;
	HMM(int pool_capacity, double sigma, double tau, double eta){
		_pool_capacity = pool_capacity;
		_sampling_table = new double[pool_capacity];
		_sigma = sigma;
		_tau = tau;
		_eta = eta;
	}
	~HMM(){
		delete[] _sampling_table;
		for(int t = 0;t < _pool.size();t++){
			delete[] _pool[t];
		}
	}
	void fill_pool(vector<double> &prev_state_sequence){
		int seq_length = prev_state_sequence.size();
		for(int t = 0;t < seq_length;t++){
			double* pool = new double[_pool_capacity];
			pool[0] = prev_state_sequence[t];	// prev state is always in the pool
			for(int k = 1;k < _pool_capacity;k++){
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
		fill_pool(sampled_state_sequence);
		vector<double*> u;
		vector<double> zu;
		vector<double*> w;
		vector<double> wu;
		int seq_length = output_sequence.size();
		// forward
		for(int t = 0;t < seq_length;t++){
			double yt = output_sequence[t];
			double* xt = _pool[t];
			double* ut = new double[_pool_capacity];
			double zut = 0;
			for(int j = 0;j < _pool_capacity;j++){
				ut[j] = compute_observation_probability(yt, xt[j]) / compute_pool_probability(xt[j]);
				zut += ut[j];
			}
			// normalize
			for(int j = 0;j < _pool_capacity;j++){
				ut[j] /= zut;
			}
			u.push_back(ut);
			zu.push_back(zut);
		}

		double* wt = new double[_pool_capacity];
		for(int j = 0;j < _pool_capacity;j++){
			wt[j] = u[0][j];
		}
		w.push_back(wt);

		for(int t = 1;t < seq_length;t++){
			double* xt_1 = _pool[t - 1];
			double* xt = _pool[t];
			double* wt_1 = w[t - 1];
			double* wt = new double[_pool_capacity];
			double* ut = u[t];
			double zwt = 0;
			for(int j = 0;j < _pool_capacity;j++){
				double sum = 0;
				for(int k = 0;k < _pool_capacity;k++){
					sum += wt_1[k] * compute_transition_probability(xt[j], xt_1[k]);
				}
				wt[j] = ut[j] * sum;
				zwt += wt[j];
			}
			// normalize
			for(int j = 0;j < _pool_capacity;j++){
				wt[j] /= zwt;
			}
			w.push_back(wt);
		}
		// backward
		vector<double> s;
		int t = seq_length - 1;
		double sum = 0;
		for(int j = 0;j < _pool_capacity;j++){
			_sampling_table[j] = w[t][j];
			sum += _sampling_table[j];
		}
		double normalizer = 1.0 / sum;
		double bernoulli = sampler::uniform(0, 1);
		double stack = 0;
		for(int j = 0;j < _pool_capacity;j++){
			stack += _sampling_table[j] * normalizer;
			if(bernoulli <= stack){
				s.push_back(j);
				break;
			}
		}
		assert(s.size() == 1);
		for(int t = seq_length - 1;t >= 1;t--){
			double sum = 0;
			double* xt_1 = _pool[t - 1];
			double* xt = _pool[t];
			double* wt_1 = w[t - 1];
			int st = s[seq_length - t - 1];
			for(int j = 0;j < _pool_capacity;j++){
				_sampling_table[j] = wt_1[j] * compute_transition_probability(xt[st], xt_1[j]);
				sum += _sampling_table[j];
			}
			double normalizer = 1.0 / sum;
			double bernoulli = sampler::uniform(0, 1);
			double stack = 0;
			for(int j = 0;j < _pool_capacity;j++){
				stack += _sampling_table[j] * normalizer;
				if(bernoulli <= stack){
					s.push_back(j);
					break;
				}
			}
			assert(s.size() == seq_length - t + 1);
		}

		std::reverse(s.begin(), s.end());
		for(int t = 0;t < seq_length;t++){
			int index = s[t];
			assert(index < _pool_capacity);
			double state = _pool[t][index];
			sampled_state_sequence[t] = state;
		}

		for(int t = 0;t < seq_length;t++){
			delete[] u[t];
			delete[] w[t];
		}

	}
};