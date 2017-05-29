#include <boost/python.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <cmath>
#include "src/hmm.h"
#include "src/sampler.h"
using namespace boost;

class PyHMM{
public:
	PyHMM(int pool_capacity){
		setlocale(LC_CTYPE, "ja_JP.UTF-8");
		ios_base::sync_with_stdio(false);
		locale default_loc("ja_JP.UTF-8");
		locale::global(default_loc);
		locale ctype_default(locale::classic(), default_loc, locale::ctype); //※
		wcout.imbue(ctype_default);
		wcin.imbue(ctype_default);
	}
	void generate_sequence(vector<double> &state_sequence, vector<double> &output_sequence, int seq_length, double sigma, double eta, double tau){
		double xt = 0, yt;
		for(int t = 1;t < seq_length;t++){
			xt = sampler::normal(tanh(eta * xt), tau);
			yt = sampler::normal(xt, sigma);
			state_sequence.push_back(xt);
			output_sequence.push_back(yt);
		}
	}
};

BOOST_PYTHON_MODULE(model){
	python::class_<PyHMM>("hmm", python::init<int>());
}