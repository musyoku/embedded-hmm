#include <boost/python.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <cmath>
#include "src/hmm.h"
#include "src/sampler.h"
using namespace boost;

class PyHMM{
private:
	HMM* _hmm;
public:
	PyHMM(int pool_capacity, double sigma, double tau, double eta){
		setlocale(LC_CTYPE, "ja_JP.UTF-8");
		ios_base::sync_with_stdio(false);
		locale default_loc("ja_JP.UTF-8");
		locale::global(default_loc);
		locale ctype_default(locale::classic(), default_loc, locale::ctype); //※
		wcout.imbue(ctype_default);
		wcin.imbue(ctype_default);
		_hmm = new HMM(pool_capacity, sigma, tau, eta);
	}
	void generate_sequence(vector<double> &state_sequence, vector<double> &output_sequence, int seq_length){
		double xt = 0, yt;
		for(int t = 0;t < seq_length;t++){
			xt = sampler::normal(tanh(_hmm->_eta * xt), _hmm->_tau);
			yt = sampler::normal(xt, _hmm->_sigma);
			state_sequence.push_back(xt);
			output_sequence.push_back(yt);
		}
	}
	void perform_forward_backward(vector<double> &output_sequence, vector<double> &sampled_state_sequence){
		assert(output_sequence.size() > 0);
		assert(sampled_state_sequence.size() == output_sequence.size());
		_hmm->perform_forward_backward(output_sequence, sampled_state_sequence);
	}
};

BOOST_PYTHON_MODULE(model){
	python::class_<PyHMM>("hmm", python::init<int, double, double, double>());
}