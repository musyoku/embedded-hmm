#pragma once
#include <vector>
#include "sampler.h"

class HMM{
private:
	vector<double*> _pool;
	int _pool_capacity;
public:
	HMM(int pool_capacity){
		_pool_capacity = 0;
	}
	void init_pool(int seq_length){
		for(int t = 0;t < seq_length;t++){
			double* pool = new double[_pool_capacity];
			for(int k = 0;k < _pool_capacity;k++){
				pool[k] = sampler::normal(0, 1);
			}
		}	
	}
	void perform_forward_backward(vector<double> &output_sequence, vector<double> &sampled_state_sequence){
		init_pool(output_sequence.size());
	}
};