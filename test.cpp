#include "src/hmm.h"
#include "model.cpp"

int main(int argc, char *argv[]){
	// 日本語周り
	setlocale(LC_CTYPE, "");
	ios_base::sync_with_stdio(false);
	locale default_loc("");
	locale::global(default_loc);
	locale ctype_default(locale::classic(), default_loc, locale::ctype);
	wcout.imbue(ctype_default);
	wcin.imbue(ctype_default);

	PyHMM* model = new PyHMM(10, 2.5, 0.4, 2.5);
	vector<double> state_sequence;
	vector<double> output_sequence;
	int seq_length = 100;
	model->generate_sequence(state_sequence, output_sequence, seq_length);
	cout << state_sequence.size() << endl;
	delete model;

}